import common.geogebra as geogebra
from common.avoidance.obstaclemap import *

from common.avoidance.obstacles import *
from common.avoidance.funct import *
import common.avoidance.geometry as geometry

import math
import time
import os


TEST = False
FILE = False
PRINT = False

if TEST:
    from robots.setup_wheeledbase import *
    linvel = 500 #wheeledbase.get_parameter_value(POSITIONCONTROL_LINVELMAX_ID, FLOAT)/4
    angvel = wheeledbase.get_parameter_value(POSITIONCONTROL_ANGVELMAX_ID, FLOAT)
else:
    linvel = 600
    angvel = -1


print("linvel : ", linvel)
print("angvel : ", angvel)

#all
geo = geogebra.Geogebra("test.ggb")
robot = geometry.Point(*geo.get("origin"))
goal = geometry.Point(*geo.get("goal"))
max_w_pf = 2
robot_width = 300

obsmap = ObstacleMap()

funct = funct_list["lin"](a=max_w_pf/robot_width, b=max_w_pf)
for obs in geo.getall("obsc_*"):
    obsmap.add_obstacle(DiskObs(Point(obs[0], obs[1]), obs[2], funct))
poly_in = geo.getall("obsin_*")
for poly in poly_in:
    obsmap.add_obstacle(PolygonObs(poly, funct))
maps = geometry.Point(*geo.get("MAP"))
obsmap.add_obstacle(Map(maps.x, maps.y, funct))

#ftg
alpha_static = 300
distance_max = 800
# poly_out = geo.getall("obsout_*")
# print(poly_out)
# for poly in poly_in:
#     obsmap.add_obstacle_ftg(PolygonObs(poly, funct))

#pf
objs = list()
#obsmap.add_obstacle_pf(PointObs(goal.x, goal.y, funct_list["lin"](a=-max_w_pf/robot_width, b=-max_w_pf)))


timestep = 0.2
step = linvel * timestep
path = []
nb_pts = 0

if TEST:
    wheeledbase.set_position(robot.x, robot.y, -math.pi/2)

max_pts = 100

with open("list_point", "w") as file:
    if FILE:
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
        w_pf = abs(w_pf)
        v_pf = min(1.0, max_w_pf-w_pf)

        # all
        angle_guide = obsmap.angle_average(angle_guide_pf, angle_guide_ftg, w1=w_pf, w2=w_ftg)
        v = (v_pf + v_ftg) / 2 * linvel

        diff = time.time() - begin

        # Indicate angle to wheeledbase
        if TEST:
            wheeledbase.follow_angle(angle_guide, v)

        # Update position
        if TEST:
            robot_f = geometry.Point(wheeledbase.get_position()[0], wheeledbase.get_position()[1])
        else:
            dx = math.cos(angle_guide) * v * timestep
            dy = math.sin(angle_guide) * v * timestep
            robot_f = geometry.Point(robot.x + dx, robot.y + dy)
        path += [(robot_f.x, robot_f.y)]

        # Display debug information
        if PRINT:
            print("nb : ", nb_pts)
            print("time : ", diff)
            print("angle ftg : ", angle_guide_ftg*180/pi)
            print("angle pf : ", angle_guide_pf*180/pi)
            print("angle : ", angle_guide*180/pi)
            print("w_ftg : ", w_ftg)
            print("w_pf : ", w_pf)
            print("v ftg : ", v_ftg)
            print("v pf : ", v_pf)
            print("v : ", v)
            print("ROBOT : " + "(" + str(round(robot.x)) + ", " + str(round(robot.y)) + ")")
            print()

        # write geogebra debug information in file
        if FILE:
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

        robot = robot_f
        nb_pts += 1

    if TEST:
        wheeledbase.stop()

    # Write Polyline
    if FILE:
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


