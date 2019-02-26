from common.avoidance.geometry import *


class PointObs(Point):
    def __init__(self, x, y, funct):
        self.funct = funct
        super().__init__(x, y)

    def get_force(self, position):
        x_ext, y_ext = position
        vect = (x_ext - self.x, y_ext - self.y)
        if hypot(*vect) == 0:
            return [0, 0]
        vect = vect[0] / hypot(*vect), vect[1] / hypot(*vect)
        return self.funct(vect, hypot(x_ext - self.x, y_ext - self.y))

    def get_scalar(self, position):
        x_ext, y_ext = position
        return self.funct.apply(hypot(x_ext-self.x, y_ext-self.y))


class PolygonObs(Polygon):
    def __init__(self, vertices, funct):
        self.funct = funct
        super().__init__(vertices)
        self.vertices = [self.vertices[-1]] + self.vertices + [self.vertices[0]]

    def get_force(self, position, scalar=False):
        x_ext, y_ext = position
        min_i = -1
        distance_min = 60000
        for i in range(1, len(self.vertices) - 1):
            x, y = self.vertices[i]
            distance = hypot(x - x_ext, y - y_ext)
            if distance < distance_min:
                distance_min = distance
                min_i = i

        distances_mur = [0, 0]
        for k in range(2):
            x1, y1 = self.vertices[min_i - 1 + k]
            x2, y2 = self.vertices[min_i + k]
            cote = (x2 - x1, y2 - y1)
            robot = (x_ext - x1, y_ext - y1)
            scalaire = robot[0] * cote[0] + robot[1] * cote[1]
            projection = scalaire / hypot(*cote)
            distance_du_mur = (robot[0] * cote[1] - cote[0] * robot[1]) / hypot(*cote)
            distances_mur[k] = distance_du_mur
            if 0 <= projection <= hypot(*cote) and distance_du_mur >= 0:
                vect = [sin(atan2(cote[1], cote[0])), -1 * cos(atan2(cote[1], cote[0]))]
                if isclose(0, distance_du_mur, abs_tol=0.1):
                    if scalar:
                        return self.funct.get_max()
                    else:
                        return self.funct.saturate(vect)
                if scalar:
                    return self.funct.apply(distance_du_mur)
                else:
                    return self.funct(vect, distance_du_mur)

        if distances_mur[0] * distances_mur[1] > 0 and distances_mur[0] < 0:
            # if point is on polygon
            center = [0, 0]
            for x, y in self.vertices:
                center[0] += x / len(self.vertices)
                center[1] += y / len(self.vertices)
            vect = [(x_ext - center[0]), (y_ext - center[1])]
            vect = [vect[0] / hypot(*vect), vect[1] / hypot(*vect)]

            if scalar:
                return self.funct.get_max()
            else:
                return self.funct.saturate(vect)

        vect = [(x_ext - self.vertices[min_i][0]), (y_ext - self.vertices[min_i][1])]
        vect = [vect[0] / hypot(*vect), vect[1] / hypot(*vect)]
        if scalar:
            return self.funct.apply(distance_min)
        else:
            return self.funct(vect, distance_min)

    def get_scalar(self, position):
        scalar = self.get_force(position, scalar=True)
        return scalar


class Map(PolygonObs):
    def __init__(self, width, height, funct):
        self.height = height
        self.width = width
        super().__init__([(0, 0), (self.width, 0), (self.width, self.height), (0, self.height)], funct)

    def get_force(self, position, scalar=False):
        x, y = position
        wall_vects = ((0, y), (x, 0), (self.width, y), (x, self.height))
        norm_vects = ((1, 0), (0, 1), (-1, 0), (0, -1))
        dist = min(x, y, self.width - x, self.height - y)

        if scalar:
            return self.funct.apply(dist)
        else:
            result = (0, 0)
            for (wall_vect, norm_vect) in zip(wall_vects, norm_vects):
                (x_wall, y_wall) = wall_vect
                f = self.funct(norm_vect, hypot(x_wall - x, y_wall - y))
                result = (f[0] + result[0], f[1] + result[1])
            return result

    def get_scalar(self, position):
        return self.get_force(position, scalar=True)


class DiskObs(Disk):
    def __init__(self, center, radius, funct):
        self.funct = funct
        super().__init__(center, radius)

    def get_force(self, position):
        x_ext, y_ext = position
        vect = (x_ext - self.center.x, y_ext - self.center.y)
        if hypot(*vect) == 0:
            return [0, 0]
        vect = vect[0] / hypot(*vect), vect[1] / hypot(*vect)
        return self.funct(vect, hypot(x_ext - self.center.x, y_ext - self.center.y) - self.radius)

    def get_scalar(self, position):
        x_ext, y_ext = position
        return self.funct.apply(max(0, hypot(x_ext-self.center.x, y_ext-self.center.y) - self.radius))
