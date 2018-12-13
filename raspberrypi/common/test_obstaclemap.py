from common.obstaclemap import *
from shapely.geometry import *
from common.geogebra import *
import math
from statistics import *
import time

TEST=True
if TEST:
    from robots.setup_wheeledbase import *

geo = Geogebra("test_obstacle.ggb")

robot = Point(geo.get("origin"))

goal = Point(geo.get("goal"))


obsmap = ObstacleMap.load(geo)

alpha_static = 600
robot_width = 300
step = 200
velocity = 500
path = []
i = 0

if TEST:
    wheeledbase.set_position(300 + 110, 2550+55, -math.pi/2)

# obsmap.add_obstacle([(0, 2500), (200, 2500), (200, 2300), (0, 2300)], vel=Velocity(500, 0))
with open("list_point", "w") as file:
    file.write("Execute[{")
    while i < 100 and robot.distance(goal) > step:
        begin = time.time()
        angle_guide = obsmap.get_angle_guide(robot, goal, distance_max=1000, alpha_static=alpha_static, min_width=robot_width)
        print("\ntime : ", time.time() - begin)
        if angle_guide is None:
            break

        dx = math.cos(angle_guide) * step
        dy = math.sin(angle_guide) * step
        path += [(robot.x, robot.y)]
        file.write("\"P_{" + str(i) + "} = (" + str(round(robot.x)) + ", " +
                   str(round(robot.y)) + ")\",")
        i += 1

        if TEST:
            robot_angle_origin = wheeledbase.get_position()[2] % (2*math.pi)
            angle_rel = (angle_guide - robot_angle_origin) % (2*math.pi)
            angle_rel = angle_rel if angle_rel < math.pi else math.pi - angle_rel
            #dx_rel = math.cos(angle_rel) * step
            #dy_rel = math.sin(angle_rel) * step
            #print("delta = dx : ", dx_rel, "    dy : ", dy_rel)
            print("angle_robot = ", robot_angle_origin)
            print("angle_guide = ", angle_guide)
            print("angle_rel = ", angle_rel)

            begin = time.time()
            wheeledbase.set_velocities(100, math.copysign(0.5, angle_rel))

            if angle_rel > 0:
                while (wheeledbase.get_position()[2] - robot_angle_origin) % (2*math.pi) < angle_rel:
                    print("angle_diff : ", (wheeledbase.get_position()[2] - robot_angle_origin) % (2*math.pi))
                    time.sleep(0.01)
            else:
                while (robot_angle_origin - wheeledbase.get_position()[2]) % (2 * math.pi) < -angle_rel:
                    print("angle_diff : ", (robot_angle_origin - wheeledbase.get_position()[2]) % (2 * math.pi))
                    time.sleep(0.01)


            wheeledbase.set_velocities(100, 0)
            while time.time() - begin < 0.3:
                time.sleep(0.01)
            #wheeledbase.goto_delta(dx_rel, dy_rel)


        #obsmap.run(step/velocity)

        if TEST:
            robot = Point(wheeledbase.get_position()[0], wheeledbase.get_position()[1])
        else:
            robot = Point(robot.x + dx, robot.y + dy)
    if TEST:
        wheeledbase.stop()
    file.seek(0, os.SEEK_END)
    file.seek(file.tell() - 1, os.SEEK_SET)
    file.write("}]")

print("fin")
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




file = open("list_point_filtered", "w")
file.write("Execute[{")
for (x, y) in pts_f:
    file.write("\"(" + str(round(x)) + ", " + str(round(y)) + ")\",")
file.seek(0, os.SEEK_END)
file.seek(file.tell() - 1, os.SEEK_SET)
file.write("}]")

file.close()


