from common.obstaclemap import *
from shapely.geometry import *
from common.geogebra import *
import math

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
step = 50
path = []
i = 0
while i < 500: #robot.distance(goal) > 10:
    angle_guide = obsmap.get_angle_guide(robot, goal)
    if angle_guide is None:
        print("error")
        break
    #print("angle guide : ", angle_guide)
    robot = Point(robot.x + math.cos(angle_guide) * step, robot.y + math.sin(angle_guide) * step)
    path += [(robot.x, robot.y)]
    i += 1

#import matplotlib.pyplot as plt
#plt.plot([x for (x, y) in path], [y for (x, y) in path])
#plt.show()

file = open("list_point", "w")
file.write("Execute[{")
for (x, y) in path:
    file.write("\"(" + str(round(x)) + ", " + str(round(y)) + ")\",")
file.write("}]")

file.close()


