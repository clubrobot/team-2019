from common.obstaclemap import *
from shapely.geometry import *
from common.geogebra import *
import math
from statistics import *

geo = Geogebra("test_obstacle.ggb")

robot = Point(geo.get("origin"))

goal = Point(geo.get("goal"))


obsmap = ObstacleMap.load(geo)

alpha_static = 800
robot_width = 300
step = 200
path = []
i = 0
while i < 100 and robot.distance(goal) > step:
    angle_guide = obsmap.get_angle_guide(robot, goal, distance_max=1000, alpha_static=alpha_static, min_width=robot_width)

    if angle_guide is None:
        break
    robot = Point(robot.x + math.cos(angle_guide) * step, robot.y + math.sin(angle_guide) * step)
    path += [(robot.x, robot.y)]
    i += 1

pts = [Point(p) for p in path]
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
    window = Polygon(
        [(p.x - robot_width, p.y - robot_width), (p.x + robot_width, p.y - robot_width),
         (p.x + robot_width, p.y + robot_width), (p.x - robot_width, p.y + robot_width)])
    for k in range(nb_pts):
        if not done[k] and window.contains(pts[k]):
            done[k] = True
            pts_win += [pts[k]]
    pts_f += [(mean([p.x for p in pts_win]), mean([p.y for p in pts_win]))]


file = open("list_point", "w")
file.write("Execute[{")
i = 0
for (x, y) in [(p.x, p.y) for p in pts]:
    file.write("\"P_{" + str(i) + "} = (" + str(round(x)) + ", " + str(round(y)) + ")\",")
    i += 1
file.seek(0, os.SEEK_END)
file.seek(file.tell() - 1, os.SEEK_SET)
file.write("}]")
file.close()

file = open("list_point_filtered", "w")
file.write("Execute[{")
for (x, y) in pts_f:
    file.write("\"(" + str(round(x)) + ", " + str(round(y)) + ")\",")
file.seek(0, os.SEEK_END)
file.seek(file.tell() - 1, os.SEEK_SET)
file.write("}]")

file.close()


