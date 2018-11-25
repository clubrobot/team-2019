from shapely.geometry import *
from common.geogebra import Geogebra
import math
from shapely.affinity import *
class ObstacleMap:
    nb_phi = 100
    nb_r = 300
    def __init__(self, polygons=list()):
        self.obstacles = [Polygon(p) for p in polygons]
        print(len(self.obstacles), "polygones")

    def add_obstacle(self, polygon):
        self.obstacles += [Polygon(polygon)]

    #@profile
    def get_polar_histo(self, p, distance_max):
        histo = ([x/self.nb_phi*2*math.pi for x in range(0, self.nb_phi)],
                 [None for i in range(0, self.nb_phi)])
        print(histo[0])
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
            gaps[0] = (start, gaps[0][1]+self.nb_phi)
        return gaps

    def get_angle_of_gap(self, gap):
        return ((gap[1] + gap[0])/2 % self.nb_phi) / self.nb_phi * 2*math.pi

    def is_angle_in_gap(self, angle, gap):
        return gap[0] <= angle < gap[1]

    def get_admissible_gaps(self, histo, min_width):
        return [gap for gap in self.get_gaps(histo)
                if self.get_gap_width(histo, gap) >= min_width]



    def get_gap_width(self, histo, gap):
        p1 = Point(math.cos(histo[0][(gap[0]-1)%self.nb_phi]) * histo[1][(gap[0]-1)%self.nb_phi],
                   math.sin(histo[0][(gap[0]-1)%self.nb_phi]) * histo[1][(gap[0]-1)%self.nb_phi])
        p2 = Point(math.cos(histo[0][(gap[1]+1)%self.nb_phi]) * histo[1][(gap[1]+1)%self.nb_phi],
                   math.sin(histo[0][(gap[1]+1)%self.nb_phi]) * histo[1][(gap[1]+1)%self.nb_phi])
        return p1.distance(p2)



    @staticmethod # Factory function
    def load(geogebra, pattern="poly*"):
        if isinstance(geogebra, str):
            geogebra = Geogebra(geogebra)
            geogebra.close()

        polygons = geogebra.getall(pattern)

        return ObstacleMap(polygons)



