from shapely.geometry import *
from common.geogebra import Geogebra
import math
from shapely.affinity import *
class ObstacleMap:
    nb_phi = 70
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

    def get_middle_of_gap_basic(self, gap):
        return ((gap[1] + gap[0])/2 % self.nb_phi) / self.nb_phi * 2*math.pi

    def get_middle_of_gap(self, histo, gap):
        print("MIDDLE GAP : ", [(x * 360/self.nb_phi) for x in gap])
        p1, p2 = self.get_nearest_points_of_gap(histo, gap)
        print("\t", p1, p2)
        if p1 is None and p2 is None:
            return None
        d1 = p1.distance(Point([0, 0]))
        d2 = p2.distance(Point([0, 0]))
        phi_1 = math.atan2(p1.y, p1.x) % (2*math.pi)
        phi_2 = math.atan2(p2.y, p2.x) % (2*math.pi)
        print("\tphi_1 : ", phi_1*180/math.pi)
        print("\tphi_2 : ", phi_2*180/math.pi)
        print("\td1 : ", d1)
        print("\td2 : ", d2)

        p_middle = Point((p1.x + p2.x)/2, (p1.y + p2.y) / 2)

        angle = math.atan2(p_middle.y, p_middle.x) % (2*math.pi)
        print("\tmiddle_of_gap : ", angle*180/math.pi)
        print("\tmiddle_of_gap_basic : ", self.get_middle_of_gap_basic(gap) * 180 / math.pi)
        return angle

    @staticmethod
    def is_angle_in_gap(angle, gap):
        return gap[0] <= angle < gap[1]

    def get_admissible_gaps(self, histo, min_width):
        return [gap for gap in self.get_gaps(histo)
                if self.get_gap_width(histo, gap) >= min_width]

    def get_nearest_gap(self, gaps, angle_to_goal):
        if not gaps:
            return None
        value = self.angle_difference(self.get_middle_of_gap_basic(gaps[0]), angle_to_goal) * \
                (1 if self.last_angle_guide is None else
                 self.angle_difference(self.get_middle_of_gap_basic(gaps[0]), self.last_angle_guide))
        nearest = gaps[0]
        best_value = value
        for gap in gaps:
            value = self.angle_difference(self.get_middle_of_gap_basic(gap), angle_to_goal) * \
                    (1 if self.last_angle_guide is None else
                     self.angle_difference(self.get_middle_of_gap_basic(gap), self.last_angle_guide))
            if value < best_value:
                nearest = gap
                best_value = value
        return nearest

    def get_nearest_points_of_gap(self, histo, gap):
        min_distance = self.INFINITE
        i = gap[0] - 1
        #print("\tangle_of_gap = ", self.get_angle_of_gap(gap) * 180 / math.pi)
        p1_nearest = None
        p2_nearest = None
        while histo[1][i] is not None and self.angle_difference(self.get_middle_of_gap_basic(gap), histo[0][i]) < math.pi / 2:
            j = (gap[1] + 1) % self.nb_phi
            while histo[1][j] is not None and \
                    self.angle_difference(self.get_middle_of_gap_basic(gap), histo[0][j]) < math.pi / 2:
                p1 = Point(math.cos(histo[0][i % self.nb_phi]) * histo[1][i % self.nb_phi],
                           math.sin(histo[0][i % self.nb_phi]) * histo[1][i % self.nb_phi])
                p2 = Point(math.cos(histo[0][j % self.nb_phi]) * histo[1][j % self.nb_phi],
                           math.sin(histo[0][j % self.nb_phi]) * histo[1][j % self.nb_phi])

                distance = p1.distance(p2)
                #print("distance between : ", (round(p1.x), round(p1.y)), " and ", (round(p2.x), round(p2.y)), " = ", distance)
                if distance < min_distance:
                    p1_nearest = p1
                    p2_nearest = p2
                    min_distance = distance
                j = (j + 1) % self.nb_phi
            i = (i - 1) % self.nb_phi

        return p1_nearest, p2_nearest

    def get_gap_width(self, histo, gap):
        if abs(gap[0] - gap[1]) >= self.nb_phi/2:
            return self.INFINITE

        p1, p2 = self.get_nearest_points_of_gap(histo, gap)

        if not p1 and not p2:
            return self.INFINITE

        distance = p1.distance(p2)

        print("\tgap_width : ", gap, " = ", distance)

        return distance

    def get_angle_guide(self, robot, goal, min_width=300, distance_max=500, alpha_mobile=500, alpha_static=200.0):
        distance_max = min(robot.distance(goal), distance_max)

        histo = self.get_polar_histo(robot, distance_max)

        # import matplotlib.pyplot as plt
        # plt.plot(histo[0], histo[1])
        # plt.show()

        admissible_gaps = self.get_admissible_gaps(histo, min_width)
        print("gaps : ", [(gap[0] * 360/self.nb_phi, gap[1] * 360/self.nb_phi) for gap in self.get_gaps(histo)])
        print("admissible_gap = ", [self.get_middle_of_gap_basic(gap) * 360 / 2 / math.pi for gap in admissible_gaps])

        if not admissible_gaps:
            print("no admissible gaps")
            return None

        histo_not_none = [x for x in histo[1] if x is not None]
        if not histo_not_none:
            d_min = self.INFINITE
        else: d_min = min(histo_not_none)
        print("d_min = ", d_min)

        angle_to_goal = math.atan2(goal.y - robot.y, goal.x - robot.x) % (2*math.pi)

        print("angle_to_goal = ", angle_to_goal*360/2/math.pi)

        gap = self.get_nearest_gap(admissible_gaps, angle_to_goal)

        angle_to_gap = self.get_middle_of_gap(histo, gap)
        if angle_to_gap is not None:
            print("angle_to_gap = ", angle_to_gap * 360/2/math.pi)
            print("angle_to_gap_weight = ", alpha_static/d_min)

        if angle_to_gap is not None:
            angle_guide = self.angle_average(angle_to_goal, angle_to_gap, w1=1, w2=(alpha_static/d_min)**2)
        else:
            angle_guide = angle_to_goal

        print("angle_guide = ", angle_guide * 360 / 2 / math.pi)
        if self.last_angle_guide is not None:
            angle_guide = self.angle_average(angle_guide, self.last_angle_guide,
                                             w2=max(0.5, min(5, d_min/alpha_static)))
            print("last_angle_guide = ", self.last_angle_guide * 360 / 2 / math.pi)

        print("new_angle_guide = ", angle_guide * 360 / 2 / math.pi)

        #if self.last_angle_guide is not None:
        #    print("last_angle_guide = ", self.last_angle_guide * 360 / 2 / math.pi)
        #if self.last_angle_guide is not None and self.angle_difference(self.last_angle_guide, angle_guide) > 3*math.pi/4:
        #    angle_guide = (angle_guide + math.pi) % (2*math.pi)

        self.last_angle_guide = angle_guide
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



