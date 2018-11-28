from shapely.geometry import *
from common.geogebra import Geogebra
import math
from shapely.affinity import *
class ObstacleMap:
    nb_phi = 100
    nb_r = 20
    INFINITE = 100000

    last_angle_guide = None

    def __init__(self, polygons=list()):
        self.obstacles = [Polygon(p) for p in polygons]
        #print(len(self.obstacles), "polygones")

    def add_obstacle(self, polygon):
        self.obstacles += [Polygon(polygon)]

    #@profile
    def get_polar_histo(self, p, distance_max):
        histo = ([x/self.nb_phi*2*math.pi for x in range(0, self.nb_phi)],
                 [None for i in range(0, self.nb_phi)])
        #print(histo[0])
        for phi_i in range(self.nb_phi):
            p2 = Point(p)
            phi = phi_i / self.nb_phi * 2 * math.pi
            obstacle = False
            dx = math.cos(phi)*distance_max/self.nb_r
            dy = math.sin(phi)*distance_max/self.nb_r
            for r in range(self.nb_r):
                p2 = translate(p2, dx, dy)#Point(p.x + dx*r, p.y + dy*r)
                for obs in self.obstacles:
                    if obs.contains(p2):
                        histo[1][phi_i] = p2.distance(p)
                        obstacle = True
                        break
                if obstacle:
                    break
        return histo

    def get_gaps(self, histo):
        gaps = []
        START = 0
        END = 1
        state = START
        for phi_i in range(len(histo[0])):
            if state is START:
                if histo[1][phi_i] is None:
                    start = phi_i
                    state = END
            elif state is END:
                if histo[1][phi_i] is not None:
                    end = phi_i -1
                    gaps += [(start, end)]
                    state = START

        if state is END:
            if len(gaps) >= 1:
                gaps[0] = (start, gaps[0][1]+self.nb_phi)
            else:
                gaps += [(start, self.nb_phi)]
        return gaps

    def get_angle_of_gap(self, gap):
        return ((gap[1] + gap[0])/2 % self.nb_phi) / self   .nb_phi * 2*math.pi

    @staticmethod
    def is_angle_in_gap(angle, gap):
        return gap[0] <= angle < gap[1]

    def get_admissible_gaps(self, histo, min_width):
        return [gap for gap in self.get_gaps(histo)
                if self.get_gap_width(histo, gap) >= min_width]

    def get_nearest_gap(self, gaps, angle):
        nearest = None
        for gap in gaps:
            if nearest is None or \
               abs(self.get_angle_of_gap(gap) - angle) < abs(self.get_angle_of_gap(nearest) - angle):
                nearest = gap
        return nearest

    def get_gap_width(self, histo, gap):
        if abs(gap[0] - gap[1]) > self.nb_phi/2:
            return self.INFINITE
        p1 = Point(math.cos(histo[0][(gap[0]-1) % self.nb_phi]) * histo[1][(gap[0]-1) % self.nb_phi],
                   math.sin(histo[0][(gap[0]-1) % self.nb_phi]) * histo[1][(gap[0]-1) % self.nb_phi])
        p2 = Point(math.cos(histo[0][(gap[1]+1) % self.nb_phi]) * histo[1][(gap[1]+1) % self.nb_phi],
                   math.sin(histo[0][(gap[1]+1) % self.nb_phi]) * histo[1][(gap[1]+1) % self.nb_phi])
        return p1.distance(p2)

    def get_angle_guide(self, robot, goal, min_width=500, distance_max=None, alpha=500):
        if distance_max is None:
            distance_max = robot.distance(goal)

        histo = self.get_polar_histo(robot, distance_max)

        # import matplotlib.pyplot as plt
        # plt.plot(histo[0], histo[1])
        # plt.show()

        gaps = self.get_admissible_gaps(histo, min_width)

        print("\n NEW")
        print("gaps : ", [(gap[0] * 360/self.nb_phi, gap[1] * 360/self.nb_phi) for gap in self.get_gaps(histo)])
        print("admissible_gap = ", [self.get_angle_of_gap(gap)*360/2/math.pi for gap in gaps])

        histo_not_none = [x for x in histo[1] if x is not None]
        if not histo_not_none:
            d_min = self.INFINITE
        else: d_min = min(histo_not_none)
        print("d_min = ", d_min)

        angle_to_goal = math.atan2(goal.y - robot.y, goal.x - robot.x) % (2*math.pi)

        print("angle_to_goal = ", angle_to_goal*360/2/math.pi)

        gap = self.get_nearest_gap(gaps, angle_to_goal)
        print("nearest_gap = ", self.get_angle_of_gap(gap±))
        if gap is None:
            return None
        angle_to_gap = self.get_angle_of_gap(gap)

        print("angle_to_gap = ", angle_to_gap * 360/2/math.pi)
        print("angle_to_gap_weight = ", alpha/d_min)

        angle_guide = self.angle_average(angle_to_goal, angle_to_gap, 1, alpha/d_min)
        if self.last_angle_guide is not None:
            angle_guide = self.angle_average(angle_guide, self.last_angle_guide)
        print("angle_guide = ", angle_guide * 360 / 2 / math.pi)
        #if self.last_angle_guide is not None:
        #    print("last_angle_guide = ", self.last_angle_guide * 360 / 2 / math.pi)
        #if self.last_angle_guide is not None and self.angle_difference(self.last_angle_guide, angle_guide) > 3*math.pi/4:
        #    angle_guide = (angle_guide + math.pi) % (2*math.pi)

        self.last_angle_guide = angle_guide
        print("new_angle_guide = ", angle_guide * 360/2/math.pi)
        return angle_guide

    @staticmethod
    def angle_difference(a1, a2):
        abs_diff = abs(a1 - a2)
        if abs_diff > math.pi:
            return 2*math.pi - abs_diff
        else:
            return abs_diff


    @staticmethod
    def angle_average(a1, a2, w1=1, w2=1):
        x = math.cos(a1) * w1 + math.cos(a2) * w2
        y = math.sin(a1) * w1 + math.sin(a2) * w2
        return math.atan2(y, x) % (2*math.pi)

    @staticmethod # Factory function
    def load(geogebra, pattern="poly*"):
        if isinstance(geogebra, str):
            geogebra = Geogebra(geogebra)
            geogebra.close()

        polygons = geogebra.getall(pattern)

        return ObstacleMap(polygons)



