import common.geogebra as geogebra
from common.avoidance.obstaclemap import *

from common.avoidance.obstacles import *
from common.avoidance.funct import *
import common.avoidance.geometry as geometry

import math
import time
import os


TEST = False
if TEST:
    from robots.setup_wheeledbase import *
    linvel = 200 #wheeledbase.get_parameter_value(POSITIONCONTROL_LINVELMAX_ID, FLOAT)/4
    angvel = wheeledbase.get_parameter_value(POSITIONCONTROL_ANGVELMAX_ID, FLOAT)
else:
    linvel = 200
    angvel = -1


print("linvel : ", linvel)
print("angvel : ", angvel)

#all
geo = geogebra.Geogebra("test.ggb")
robot = geometry.Point(*geo.get("origin"))
goal = geometry.Point(*geo.get("goal"))
period = 1

#ftg
obsmap = ObstacleMap()
[obsmap.add_obstacle_ftg(geometry.Polygon(obs)) for obs in geo.getall("obs*")]
alpha_static = 600
robot_width = 300
distance_max = 800

#pf
objs = list()
maps = geometry.Point(*geo.get("MAP"))
polys = geo.getall("obsinp*")
disks = geo.getall("obsinc*")
max_w_pf = 10
obsmap.add_obstacle_pf(Map(2000,3000, funct_list["lin"](a=max_w_pf/robot_width, b=max_w_pf)))
# obsmap.add_obstacle_pf(PointObs(goal.x, goal.y, funct_list["lin"](a=max_w_pf/robot_width, b=max_w_pf)))
for poly in polys:
    obsmap.add_obstacle_pf(PolygonObs(poly, funct_list["lin"](a=max_w_pf/robot_width, b=max_w_pf)))

step = linvel
path = []
nb_pts = 0

if TEST:
    wheeledbase.set_position(robot.x, robot.y, math.pi/2)

max_pts = 100

# obsmap.add_obstacle([(0, 2500), (200, 2500), (200, 2300), (0, 2300)], vel=Velocity(500, 0))
with open("list_point", "w") as file:
    file.write("Execute[{")

    while nb_pts < max_pts and robot.distance(goal) > step/2:
        # follow the gap
        begin = time.time()
        ret = obsmap.get_ftg_angle_guide(robot, goal, distance_max=distance_max, alpha_static=alpha_static, min_width=robot_width)
        if ret is None:
            break
        else:
            angle_guide_ftg, n_distance = ret
            v_ftg = min(1.0, abs(n_distance)*2)
        w_ftg = v_ftg

        # potential field
        angle_guide_pf, w_pf = obsmap.get_pf_angle_guide(robot)
        w_pf = abs(w_pf/max_w_pf)
        v_pf = min(1.0, 1-w_pf)

        # all
        # if obsmap.angle_difference(angle_guide_ftg, angle_guide_pf) < 3/4*math.pi:
        #    angle_guide = obsmap.angle_average(angle_guide_pf, angle_guide_ftg, w1=v_pf, w2=v_ftg)
        # else:
        #    angle_guide = angle_guide_ftg

        angle_guide = obsmap.angle_average(angle_guide_pf, angle_guide_ftg, w1=w_pf, w2=w_ftg)

        v = (v_pf + v_ftg) / 2 * linvel
        v = min(v, linvel)

        diff = time.time() - begin

        print("nb : ", nb_pts)
        print("time : ", diff)
        print("angle ftg : ", angle_guide_ftg*180/pi)
        print("v ftg : ", v_ftg)
        print("w_ftg : ", w_ftg)
        print("angle pf : ", angle_guide_pf*180/pi)
        print("v pf : ", v_pf)
        print("w_pf : ", w_pf)
        print("angle : ", angle_guide*180/pi)
        print("v : ", v)

        path += [(robot.x, robot.y)]

        # write geogebra debug information in file

        # global path
        c_path = "path_{" + str(nb_pts) + "}"
        file.write("\"" + c_path + " = Point({" + str(round(robot.x)) + ", " + str(round(robot.y)) + "})\", \n")
        file.write("\"SetVisibleInView(" + c_path + ", 1, False)\", \n\n")

        # PF vectors
        display_pf_ratio = 100
        dx_pf = round(display_pf_ratio*w_pf * cos(angle_guide_pf))
        dy_pf = round(display_pf_ratio*w_pf * sin(angle_guide_pf))

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
            robot = geometry.Point(wheeledbase.get_position()[0], wheeledbase.get_position()[1])
        else:
            dx = math.cos(angle_guide) * v * diff*10
            dy = math.sin(angle_guide) * v * diff*10
            robot = geometry.Point(robot.x + dx, robot.y + dy)
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

if nb_pts == max_pts:
    print("goal not reached")
print("fin")

