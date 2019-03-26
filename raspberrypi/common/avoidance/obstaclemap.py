import common.geogebra as geogebra
import math
from common.avoidance.obstacle import *
import common.avoidance.geometry as geometry


class ObstacleMap:
    nb_phi = 100
    INFINITE = 100000
    last_angle_guide = None

    def __init__(self):
        self.obstacles_ftg = []
        self.obstacles_pf = []
        self.cos_phi = []
        self.sin_phi = []
        for phi_i in range(self.nb_phi):
            phi = phi_i / self.nb_phi * 2 * math.pi
            self.cos_phi += [math.cos(phi)]
            self.sin_phi += [math.sin(phi)]

    def add_obstacle(self, geom, vel=Velocity(0, 0)):
        self.obstacles_ftg += [Obstacle(geom, vel)]
        self.obstacles_pf += [Obstacle(geom, vel)]

    def add_obstacle_pf(self, geom, vel=Velocity(0, 0)):
        self.obstacles_pf += [Obstacle(geom, vel)]

    def add_obstacle_ftg(self, geom, vel=Velocity(0, 0)):
        self.obstacles_ftg += [Obstacle(geom, vel)]

    def get_polar_histo(self, p, distance_max):
        histo = ([x / self.nb_phi * 2 * math.pi for x in range(0, self.nb_phi)],
                 [None for _ in range(0, self.nb_phi)])

        for phi_i in range(self.nb_phi):
            phi = phi_i / self.nb_phi * 2 * math.pi
            dx = self.cos_phi[phi_i] * distance_max
            dy = self.sin_phi[phi_i] * distance_max
            seg = geometry.Segment(geometry.Point(p.x, p.y), geometry.Point(p.x + dx, p.y + dy))
            # print()
            # print(phi*180/math.pi)
            for obs in self.obstacles_ftg:
                # print(obs.geom)
                inter = obs.geom.segment_intersection_point(seg)
                # print(inter)
                if inter:
                    d = round(inter.distance(p))
                    if histo[1][phi_i] is None:
                        histo[1][phi_i] = d
                    histo[1][phi_i] = min(histo[1][phi_i], d)
                # print()
        #     print("\n")
        # print(histo[1])

        # import matplotlib.pyplot as plt
        # from matplotlib import cm
        # fig = plt.figure()
        # x = []
        # y = []
        # for i in range(self.nb_phi):
        #     if histo[1][i] is not None:
        #         x += [math.cos(histo[0][i])*histo[1][i]]
        #         y += [math.sin(histo[0][i])*histo[1][i]]
        # plt.plot(x, y, 'ro')
        # plt.axis([-distance_max, distance_max, -distance_max, distance_max])
        # #plt.show()
        return histo

    def get_gaps(self, histo):
        gaps = []
        start_st = 0
        end_st = 1
        state = start_st
        start = None
        for phi_i in range(len(histo[0])):
            if state is start_st:
                if histo[1][phi_i] is None:
                    start = phi_i
                    state = end_st
            elif state is end_st:
                if histo[1][phi_i] is not None:
                    end = phi_i - 1
                    gaps += [(start, end)]
                    state = start_st

        if state is end_st:
            if len(gaps) >= 1:
                gaps[0] = (start, gaps[0][1] + self.nb_phi)
            else:
                gaps += [(start, self.nb_phi)]
        return gaps

    def get_nearest_points_of_gap(self, histo, gap):
        min_distance = self.INFINITE
        i = gap[0] - 1
        j = (gap[1] + 1) % self.nb_phi
        j = (gap[1] + 1) % self.nb_phi
        p1_nearest = None
        p2_nearest = None
        while histo[1][i] is not None and \
                self.angle_difference(self.get_middle_of_gap_basic(gap), histo[0][i]) < 3 * math.pi / 4:
            j = (gap[1] + 1) % self.nb_phi
            while histo[1][j] is not None and \
                    self.angle_difference(self.get_middle_of_gap_basic(gap), histo[0][j]) < 3*math.pi / 4:
                p1 = geometry.Point(math.cos(histo[0][i]) * histo[1][i],
                           math.sin(histo[0][i]) * histo[1][i])
                p2 = geometry.Point(math.cos(histo[0][j]) * histo[1][j],
                           math.sin(histo[0][j]) * histo[1][j])

                distance = p1.distance(p2)
                if distance < min_distance:
                    p1_nearest = p1
                    p2_nearest = p2
                    min_distance = distance
                j = (j + 1) % self.nb_phi
            i = (i - 1) % self.nb_phi

        return p1_nearest, p2_nearest

    def get_middle_of_gap(self, histo, gap):
        p1, p2 = self.get_nearest_points_of_gap(histo, gap)
        if p1 is None and p2 is None:
            return None

        p_middle = geometry.Point((p1.x + p2.x) / 2, (p1.y + p2.y) / 2)

        angle = math.atan2(p_middle.y, p_middle.x) % (2 * math.pi)
        return angle

    def get_middle_of_gap_basic(self, gap):
        return ((gap[1] + gap[0]) / 2 % self.nb_phi) / self.nb_phi * 2 * math.pi

    def get_gap_width(self, histo, gap):
        if abs(gap[0] - gap[1]) >= self.nb_phi / 2:
            return self.INFINITE

        p1, p2 = self.get_nearest_points_of_gap(histo, gap)

        if not p1 and not p2:
            return self.INFINITE

        distance = p1.distance(p2)
        print(gap[0]*360/self.nb_phi, gap[1]*360/self.nb_phi, " : ", distance)
        return distance

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

    def get_admissible_gaps(self, histo, min_width):
        return [gap for gap in self.get_gaps(histo)
                if abs(gap[0] - gap[1]) > 2 and self.get_gap_width(histo, gap) >= min_width]

    def get_ftg_angle_guide(self, robot, goal, min_width=300, distance_max=500, alpha_static=200.0):
        distance_max = min(robot.distance(goal), distance_max)
        alpha_static = min(robot.distance(goal)/2, alpha_static)

        histo = self.get_polar_histo(robot, distance_max)

        admissible_gaps = self.get_admissible_gaps(histo, min_width)

        if not admissible_gaps:
            print("no gap")
            return None

        histo_not_none = [x for x in histo[1] if x is not None]
        if not histo_not_none:
            d_min = self.INFINITE
        else:
            d_min = min(histo_not_none)

        angle_to_goal = math.atan2(goal.y - robot.y, goal.x - robot.x) % (2 * math.pi)

        gap = self.get_nearest_gap(admissible_gaps, angle_to_goal)

        # angle_to_gap = self.get_middle_of_gap(histo, gap)
        angle_to_gap = self.get_middle_of_gap_basic(gap)

        if angle_to_gap is not None:
            angle_guide = self.angle_average(angle_to_goal, angle_to_gap, w1=1.0, w2=(alpha_static / d_min) ** 2)
        else:
            angle_guide = angle_to_goal

        self.last_angle_guide = angle_guide
        return angle_guide, d_min / alpha_static

    def get_pf_angle_guide(self, robot):
        v_pf = (0, 0)
        w_pf = 0
        for obj in self.obstacles_pf:
            res = obj.geom.get_force([robot.x, robot.y])
            v_pf = (v_pf[0] + res[0], v_pf[1] + res[1])
        angle_guide_pf = self.normalize_angle(math.atan2(v_pf[1], v_pf[0]))
        return angle_guide_pf, math.hypot(*v_pf)

    @staticmethod
    def load(geo, pattern="poly*"):
        if isinstance(geogebra, str):
            geo = geogebra.Geogebra(geo)
            geo.close()

        polygons = geo.getall(pattern)

        obsmap = ObstacleMap()
        for poly in polygons:
            obsmap.add_obstacle(geometry.Polygon(poly))
        return obsmap

    @staticmethod
    def angle_difference(a1, a2):
        abs_diff = abs(a1 - a2)
        if abs_diff > math.pi:
            return 2 * math.pi - abs_diff
        else:
            return abs_diff

    @staticmethod
    def normalize_angle(angle):
        while angle < 0:
            angle += 2 * math.pi
        while angle > 2 * math.pi:
            angle -= 2 * math.pi
        return angle

    @staticmethod
    def angle_average(a1, a2, w1=1.0, w2=1.0):
        x = math.cos(a1) * w1 + math.cos(a2) * w2
        y = math.sin(a1) * w1 + math.sin(a2) * w2
        return ObstacleMap.normalize_angle(math.atan2(y, x))