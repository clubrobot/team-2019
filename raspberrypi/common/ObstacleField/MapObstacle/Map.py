from math import exp, hypot

class Map:
    def __init__(self, width, length, funct):
        self.length = length
        self.width  = width
        self.funct = funct

    def get_force(self, position):
        x , y = position
        wall_vects = ((0, y), (x, 0), (self.width,y), (x,self.length))
        norm_vects = ((1, 0), (0, 1), (-1, 0), (0, -1))
        result = [0,0]
        for (wall_vect, norm_vect) in zip(wall_vects, norm_vects):
            (x_wall, y_wall) = wall_vect
            result[0] +=  self.funct(norm_vect, hypot(x_wall-x, y_wall-y))[0]
            result[1] +=  self.funct(norm_vect, hypot(x_wall-x, y_wall-y))[1]
        return result

    def get_scalaire(self, position):
        x , y = position
        wall_vects = ((0, y), (x, 0), (self.width,y), (x,self.length))
        norm_vects = ((1, 0), (0, 1), (-1, 0), (0, -1))
        result = 0
        for (wall_vect, norm_vect) in zip(wall_vects, norm_vects):
            (x_wall, y_wall) = wall_vect
            result  += self.funct.apply(hypot(x_wall-x, y_wall-y))
        return result




