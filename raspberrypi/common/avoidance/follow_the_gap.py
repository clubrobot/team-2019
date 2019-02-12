from common.geogebra import *
from common.avoidance.obstaclemap import *
import shapely.geometry

from common.avoidance.ObstacleField.PolyObstacle.Polygon import *
from common.avoidance.ObstacleField.MapObstacle.Map import *
from common.avoidance.ObstacleField.PointObstacle.Point import *
from common.avoidance.ObstacleField.funct import *

import math
import time


TEST = True
if TEST:
    from robots.setup_wheeledbase import *
    linvel = 200 #wheeledbase.get_parameter_value(POSITIONCONTROL_LINVELMAX_ID, FLOAT)/4
    angvel = wheeledbase.get_parameter_value(POSITIONCONTROL_ANGVELMAX_ID, FLOAT)
else:
    linvel = 200

#all
geo = Geogebra("test_obstacle2.ggb")
robot = shapely.geometry.Point(geo.get("origin"))
goal = shapely.geometry.Point(geo.get("goal"))
period = 1
PF_FTG_CST = 0.5

#ftg
obsmap = ObstacleMap.load(geo, pattern="obs_*")
alpha_static = 600
robot_width = 300
distance_max = 800

#pf
objs = list()
maps = geo.get("MAP")
objs.append(Map(*maps, funct_list["exp"](alpha=0.001, beta=1)))
objs.append(Point(goal.x, goal.y, funct_list["exp"](alpha=0.0001,beta=-15)))
polys = geo.getall("obs_*")
[objs.append(Polygon(poly, funct_list["exp"](alpha=0.01, beta=100))) for poly in polys]


step = linvel
path = []
nb_pts = 0

if TEST:
    wheeledbase.set_position(robot.x, robot.y, math.pi/2)

# obsmap.add_obstacle([(0, 2500), (200, 2500), (200, 2300), (0, 2300)], vel=Velocity(500, 0))
last_angle_guide = -pi/2
with open("list_point", "w") as file:
    file.write("Execute[{")

    while nb_pts < 300 and robot.distance(goal) > step/2:
        # follow the gap
        begin = time.time()
        ret = obsmap.get_ftg_angle_guide(robot, goal, distance_max=distance_max, alpha_static=alpha_static, min_width=robot_width)
        if ret is None:
            break
        else:
            angle_guide_ftg, n_distance = ret

        v_ftg = min(1.0, n_distance)
        print("angle ftg : ", angle_guide_ftg*180/pi)
        print("v ftg : ", v_ftg)

        # potential field
        vx = 0
        vy = 0
        for obj in objs:
            v = obj.get_force([robot.x, robot.y])
            vx += v[0]
            vy += v[1]
        angle_guide_pf = obsmap.normalize_angle(atan2(vy, vx))
        v_pf = math.sqrt((vx*vx + vy*vy)) / 10
        v_pf = min(1.0, v_pf)
        print("angle pf : ", angle_guide_pf*180/pi)
        print("v pf : ", v_pf)

        # all
        if obsmap.angle_difference(angle_guide_ftg, angle_guide_pf) < math.pi/2:
            angle_guide = obsmap.angle_average(angle_guide_pf, angle_guide_ftg, w1=PF_FTG_CST/n_distance)
            #angle_guide = obsmap.angle_average(angle_guide, last_angle_guide,
            #                                  w2=max(0.5, min(1.5, n_distance)))
        else:
            angle_guide = angle_guide_ftg

        last_angle_guide = angle_guide
        v = (v_pf + v_ftg) / 2 * linvel
        v = min(v, linvel)

        print("angle : ", angle_guide*180/pi)
        print("v : ", v)
        print("time : ", time.time() - begin)

        path += [(robot.x, robot.y)]

        # write geogebra debug information in file

        # global path
        c_path = "path_{" + str(nb_pts) + "}"
        file.write("\"" + c_path + " = Point({" + str(round(robot.x)) + ", " + str(round(robot.y)) + "})\", \n")
        file.write("\"SetVisibleInView(" + c_path + ", 1, False)\", \n\n")

        # PF vectors
        display_pf_ratio = 100
        dx_pf = round(display_pf_ratio*v_pf * cos(angle_guide_pf))
        dy_pf = round(display_pf_ratio*v_pf * sin(angle_guide_pf))

        file.write("\"pf_{" + str(nb_pts) + "} = Point(" + c_path + ", Vector((" + str(dx_pf) + ", " + str(dy_pf) + ")))\", \n")
        file.write("\"SetVisibleInView(" + "pf_{" + str(nb_pts) + "}, 1, False)\", \n")
        file.write("\"pfv_{" + str(nb_pts) + "} = Vector(" + c_path + ", pf_{" + str(nb_pts) + "})\", \n")
        file.write("\"SetColor(" + "pfv_{" + str(nb_pts) + "}, 255, 0, 0)\", \n")
        file.write("\"ShowLabel(" + "pfv_{" + str(nb_pts) + "}, False)\", \n\n")

        #FTG vectors
        display_ftg_ratio = 100
        dx_ftg = round(display_ftg_ratio*v_ftg * cos(angle_guide_ftg))
        dy_ftg = round(display_ftg_ratio*v_ftg * sin(angle_guide_ftg))

        file.write("\"ftg_{" + str(nb_pts) + "} = Point(" + c_path + ", Vector((" + str(dx_ftg) + ", " + str(dy_ftg) + ")))\", \n")
        file.write("\"SetVisibleInView(" + "ftg_{" + str(nb_pts) + "}, 1, False)\", \n")
        file.write("\"ftgv_{" + str(nb_pts) + "} = Vector(" + c_path + ", ftg_{" + str(nb_pts) + "})\", \n")
        file.write("\"SetColor(" + "ftgv_{" + str(nb_pts) + "}, 0, 255, 0)\", \n")
        file.write("\"ShowLabel(" + "ftgv_{" + str(nb_pts) + "}, False)\", \n\n")
        if TEST:
            try:
                wheeledbase.follow_angle(angle_guide, v)
                while not wheeledbase.isarrived():
                    time.sleep(0.05)
            except RuntimeError:
                print("spin")
                break

        if TEST:
            robot = shapely.geometry.Point(wheeledbase.get_position()[0], wheeledbase.get_position()[1])
        else:
            dx = math.cos(angle_guide) * v * period
            dy = math.sin(angle_guide) * v * period
            robot = shapely.geometry.Point(robot.x + dx, robot.y + dy)
            print("ROBOT : " + "(" + str(round(robot.x)) + ", " + str(round(robot.y)) + ")")



        print()
        nb_pts += 1

    if TEST:
        wheeledbase.stop()

    # Write Polyline
    if nb_pts > 1:
        file.write("\"polypath = Polyline(")
        for i in range(nb_pts):
            file.write("path_{"+str(i) + "},")

    file.seek(0, os.SEEK_END)
    file.seek(file.tell() - 1, os.SEEK_SET)
    file.write(")\", \"SetColor(polypath, 255, 255, 255)\"}]\n")

print("fin")


#pts = [shapely.geometry.Point(p) for p in path]
#nb_pts = i
#done = [False for _ in range(nb_pts)]
#j = 0
#pts_f = []
#while j < nb_pts:
#    while j < nb_pts and done[j]:
#        j += 1
#    if j >= nb_pts:
#        break
#    p = pts[j]
#    done[j] = True
#    pts_win = [p]
#    window = shapely.geometry.Polygon(
#        [(p.x - robot_width, p.y - robot_width), (p.x + robot_width, p.y - robot_width),
#         (p.x + robot_width, p.y + robot_width), (p.x - robot_width, p.y + robot_width)])
#    for k in range(nb_pts):
#        if not done[k] and window.contains(pts[k]):
#            done[k] = True
#            pts_win += [pts[k]]
#    pts_f += [(mean([p.x for p in pts_win]), mean([p.y for p in pts_win]))]
#
#
#file = open("list_point_filtered", "w")
#file.write("Execute[{")
#for (x, y) in pts_f:
#    file.write("\"(" + str(round(x)) + ", " + str(round(y)) + ")\",")
#file.seek(0, os.SEEK_END)
#file.seek(file.tell() - 1, os.SEEK_SET)
#file.write("}]")

#file.close()


