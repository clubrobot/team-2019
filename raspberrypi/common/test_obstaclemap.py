from common.obstaclemap import *
from shapely.geometry import *
from common.geogebra import *
import math
from statistics import *

geo = Geogebra("test_obstacle.ggb")

robot = Point(geo.get("origin"))

goal = Point(geo.get("goal"))


obsmap = ObstacleMap.load(geo)
#print("map loaded")
#histo = obsmap.get_polar_histo(Point(800, 2000), 1000)
#print(histo[1])
#gaps = obsmap.get_gaps(histo)
#print(gaps)
#for gap in gaps:
#    print(gap)
#    print("width : ", obsmap.get_gap_width(histo, gap))
#    print("angle : ", obsmap.get_angle_of_gap(gap) * 360/2/math.pi)
#
#print(obsmap.get_admissible_gaps(histo, 500))
#
max_step = 100
min_step = 10
alpha_static = 200.0
path = []
i = 0
print("start")
while i < 50 and robot.distance(goal) > max_step:
    angle_guide = obsmap.get_angle_guide(robot, goal, distance_max=1000, alpha_static=alpha_static, min_width=300)
    #step = min(max_step, max_step * (d_min/alpha_static)/2)
    #step = max(min_step, step)
    step = max_step
    if angle_guide is None:
        print("error")
        break
    #print("angle guide : ", angle_guide)
    robot = Point(robot.x + math.cos(angle_guide) * step, robot.y + math.sin(angle_guide) * step)
    path += [(robot.x, robot.y)]
    i += 1

pts = [Point(p) for p in path]
print("transformed in points")
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
        [(p.x - alpha_static*2, p.y - alpha_static*2), (p.x + alpha_static*2, p.y - alpha_static*2),
         (p.x + alpha_static*2, p.y + alpha_static*2), (p.x - alpha_static*2, p.y + alpha_static*2)])
    print("fenetre : ", window)
    for k in range(nb_pts):
        if not done[k] and window.contains(pts[k]):
            done[k] = True
            pts_win += [pts[k]]
    print(len(pts_win), " points traités")
    pts_f += [(mean([p.x for p in pts_win]), mean([p.y for p in pts_win]))]




#import matplotlib.pyplot as plt
#plt.plot([x for (x, y) in path], [y for (x, y) in path])
#plt.show()

file = open("list_point", "w")
file.write("Execute[{")
for (x, y) in [(p.x, p.y) for p in pts]:
    file.write("\"(" + str(round(x)) + ", " + str(round(y)) + ")\",")
file.write("}]")
file.close()

file = open("list_point_filtered", "w")
file.write("Execute[{")
for (x, y) in pts_f:
    file.write("\"(" + str(round(x)) + ", " + str(round(y)) + ")\",")
file.write("}]")

file.close()


