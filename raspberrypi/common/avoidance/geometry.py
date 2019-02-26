from math import *

class Point:
    def __init__(self, x, y=None):
        if y is not None:
            self.x = x
            self.y = y
        else:
            self.x = x.x
            self.y = x.y

    def __iter__(self):
        return iter([self.x, self.y])

    def __str__(self):
        return "POINT (" + str(round(self.x)) + ", " + str(round(self.y)) + ")"

    def segment_intersection_point(self, seg):
        return seg.intersection_point(self)

    def determinant(self, p2):
        return self.x*p2.y - p2.x*self.y

    def distance(self, p2):
        return hypot(p2.x - self.x, p2.y - self.y)


class Segment:
    def __init__(self, p1, p2):
        self.p1 = p1
        self.p2 = p2

    def __iter__(self):
        return iter([self.p1, self.p2])

    def __getitem__(self, item):
        if item == 0:
            return self.p1
        if item == 1:
            return self.p2
        return None

    def __str__(self):
        return "Segment (" + str(self.p1) + ", " + str(self.p2) + ")"

    def is_point_inside(self, p):
        print(p, " in ", self)
        a = self.p1
        b = self.p2
        c = p
        crossproduct = (c.y - a.y) * (b.x - a.x) - (c.x - a.x) * (b.y - a.y)
        if crossproduct > 0.001:
            print("cross : ", crossproduct)
            return False

        dotproduct = (c.x - a.x) * (b.x - a.x) + (c.y - a.y) * (b.y - a.y)
        if dotproduct < 0:
            print("dot")
            return False

        squaredlengthba = (b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y)
        if dotproduct > squaredlengthba:
            return False

        return True

    def segment_intersection_point(self, line):
        p1 = line[0]
        p2 = line[1]
        d_12 = p1.determinant(p2)
        p3 = self.p1
        p4 = self.p2
        d_34 = p3.determinant(p4)
        p_12 = Point(p1.x - p2.x, p1.y - p2.y)
        p_34 = Point(p3.x - p4.x, p3.y - p4.y)

        d_12_34 = p_12.determinant(p_34)
        if d_12_34 == 0:
            print("parallele")
            return None

        x = (d_12 * p_34.x - d_34 * p_12.x) / d_12_34
        y = (d_12 * p_34.y - d_34 * p_12.y) / d_12_34
        p = Point(x, y)
        print(p)
        if line.is_point_inside(p) and self.is_point_inside(p):
            return p
        print("not in segment")
        return None


class Polygon:
    def __init__(self, vertices):
        self.vertices = [Point(*v) for v in vertices]

    def edges(self):
        nb_points = 4
        wall_segments = []
        for i in range(nb_points):
            wall_segments += [Segment(Point(*self.vertices[i]), Point(*self.vertices[(i + 1) % nb_points]))]
        return wall_segments

    # See http://mathworld.wolfram.com/Line-LineIntersection.html
    def segment_intersection_point(self, line):
        p1 = line.p1
        first_inter = None
        for seg in self.edges():
            print(seg)
            inter = seg.segment_intersection_point(line)
            print("inter : ", inter)
            if inter is not None:
                if (first_inter is None or first_inter.distance(p1) > inter.distance(p1)):
                    print("closer than other segments")
                    first_inter = inter
        return first_inter

    def __str__(self):
        return "Polygon(" + str([v.__str__() for v in self.vertices]) + ")"


class Disk:
    def __init__(self, center, radius):
        self.center = center
        self.radius = radius

    def segment_intersection_point(self, seg):
        c = self.center
        a = seg[0]
        b = seg[1]
        dx = b.x - a.x
        dy = b.y - a.y
        dr2 = dx**2 + dy**2
        D = (a.x - c.x) * (b.y - c.y) - (b.x - c.x) * (a.y - c.y)

        delta = self.radius**2 * dr2 - D**2
        if delta >= 0:
            s_delta = sqrt(delta)
            x1 = (D*dy - dx * s_delta)/dr2 + c.x
            if min(a.x, b.x) < x1 < max(a.x, b.x):
                y1 = (-D*dx - dy*s_delta)/dr2 + c.y
                return Point(x1, y1)
            return None
        return None

    def __str__(self):
        return "(" + str(self.center) + " RADIUS " + str(round(self.radius)) + ")"
