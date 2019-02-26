from common.avoidance.geometry import *

# seg = Segment(Point(0, 2), Point(1, -1))
# polygon = Polygon([Point(0, 0), Point(0, 1), Point(1, 1), Point(1, 0)])
# i = polygon.segment_intersection_point(seg)
# print(i)
#
# disk = Disk(Point(0, 0), 1)
# i = disk.segment_intersection_point(seg)
# print(i)
#
# seg2 = Segment(Point(1, -1), Point(0, 2))
# i = disk.segment_intersection_point(seg2)
# print(i)
#
# seg2 = Segment(Point(1, -1), Point(3, 2))
# i = disk.segment_intersection_point(seg2)
# print(i)
#
# seg2 = Segment(Point(-1, 1), Point(0, -2))
# i = disk.segment_intersection_point(seg2)
# print(i)
#
# seg2 = Segment(Point(1, 1), Point(-1, 1))
# i = disk.segment_intersection_point(seg2)
# print(i)


import common.geogebra as geogebra
from common.avoidance.obstaclemap import *

from common.avoidance.obstacles import *
from common.avoidance.funct import *
import common.avoidance.geometry as geometry

#all
geo = geogebra.Geogebra("test.ggb")
robot = geometry.Point(*geo.get("origin"))
goal = geometry.Point(*geo.get("goal"))
period = 1
max_w_pf = 10
alpha_static = 600
robot_width = 300
distance_max = 1000

#ftg
obsmap = ObstacleMap()
for i in range(1,9):
    obsc = geo.get("obsc_" + str(i))
    obs = Disk(Point(obsc[0], obsc[1]), obsc[2])
    obsmap.add_obstacle_ftg(obs)
# polygons = [geometry.Disk(Point())]# [geometry.Polygon(obs) for obs in geo.getall("obsin*")]
# print(*list(map(str, [poly for poly in polygons])), sep="\n")
#[obsmap.add_obstacle_ftg(obs) for obs in polygons]

obsmap.get_ftg_angle_guide(robot, geometry.Point(2000, 3000), distance_max=distance_max, min_width=robot_width, alpha_static=alpha_static)
