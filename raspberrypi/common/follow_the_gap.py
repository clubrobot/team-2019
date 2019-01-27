from common.geogebra import *
from common.obstaclemap import *
import shapely.geometry

from common.ObstacleField.PolyObstacle.Polygon import *
from common.ObstacleField.MapObstacle.Map import *
from common.ObstacleField.PointObstacle.Point import *
from common.ObstacleField.funct import *

import math
from statistics import *
import time


TEST = False
if TEST:
    from robots.setup_wheeledbase import *

#all
geo = Geogebra("test_obstacle2.ggb")
robot = shapely.geometry.Point(geo.get("origin"))
goal = shapely.geometry.Point(geo.get("goal"))

#ftg
obsmap = ObstacleMap.load(geo, pattern="obs_*")
alpha_static = 600
robot_width = 300

#pf
objs = list()
maps = geo.get("MAP")
objs.append(Map(*maps, funct_list["exp"](alpha=0.001, beta=1)))
objs.append(Point(goal.x, goal.y, funct_list["exp"](alpha=0.0001,beta=-15)))
polys = geo.getall("obs_*")
[objs.append(Polygon(poly, funct_list["exp"](alpha=0.01, beta=100))) for poly in polys]


step = 100
path = []
i = 0

if TEST:
    wheeledbase.set_position(robot.x, robot.y, -math.pi/2)

# obsmap.add_obstacle([(0, 2500), (200, 2500), (200, 2300), (0, 2300)], vel=Velocity(500, 0))
with open("list_point", "w") as file:
    file.write("Execute[{\"path = Polyline(")
    while i < 10000 and robot.distance(goal) > step:
        #follow the gap
        begin = time.time()
        angle_guide_ftg = obsmap.get_angle_guide(robot, goal, distance_max=1000, alpha_static=alpha_static, min_width=robot_width)
        if angle_guide_ftg is None:
            break
        print(angle_guide_ftg)
        # potential field
        vx = 0
        vy = 0
        for obj in objs:
            v = obj.get_force([robot.x, robot.y])
            vx += v[0]
            vy += v[1]
        angle_guide_pf = obsmap.normalize_angle(atan2(vy, vx))
        print(angle_guide_pf)
        #all
        angle_guide = angle_guide_pf#(angle_guide_ftg + angle_guide_pf) / 2
        print(angle_guide)
        print("time : ", time.time() - begin, "\n")


        dx = math.cos(angle_guide) * step
        dy = math.sin(angle_guide) * step
        path += [(robot.x, robot.y)]
        file.write("(" + str(round(robot.x)) + ", " +
                   str(round(robot.y)) + "),")
        i += 1
        if TEST:
            wheeledbase.follow_angle(angle_guide, 100)
            while not wheeledbase.isarrived():
                time.sleep(0.1)

        if TEST:
            robot = shapely.geometry.Point(wheeledbase.get_position()[0], wheeledbase.get_position()[1])
        else:
            robot = shapely.geometry.Point(robot.x + dx, robot.y + dy)


    if TEST:
        wheeledbase.stop()


    file.seek(0, os.SEEK_END)
    file.seek(file.tell() - 1, os.SEEK_SET)
    file.write(")\", \"SetColor(path, 255, 255, 255)\"}]")

print("fin")
pts = [shapely.geometry.Point(p) for p in path]
nb_pts = i
done = [False for _ in range(nb_pts)]
j = 0
pts_f = []
while j < nb_pts:
    while j < nb_pts and done[j]:
        j += 1
    if j >= nb_pts:
        break
    p = pts[j]
    done[j] = True
    pts_win = [p]
    window = shapely.geometry.Polygon(
        [(p.x - robot_width, p.y - robot_width), (p.x + robot_width, p.y - robot_width),
         (p.x + robot_width, p.y + robot_width), (p.x - robot_width, p.y + robot_width)])
    for k in range(nb_pts):
        if not done[k] and window.contains(pts[k]):
            done[k] = True
            pts_win += [pts[k]]
    pts_f += [(mean([p.x for p in pts_win]), mean([p.y for p in pts_win]))]


file = open("list_point_filtered", "w")
file.write("Execute[{")
for (x, y) in pts_f:
    file.write("\"(" + str(round(x)) + ", " + str(round(y)) + ")\",")
file.seek(0, os.SEEK_END)
file.seek(file.tell() - 1, os.SEEK_SET)
file.write("}]")

file.close()


