from shapely.geometry import *
from shapely.affinity import *


class Velocity:
    def __init__(self, vx, vy):
        self.vx = vx
        self.vy = vy

class Obstacle:
    def __init__(self, polygon, velocity):
        self.polygon = Polygon(polygon)
        self.velocity = velocity

    def move(self, time):
        translate(self.polygon, xoff=self.velocity.vx*time, yoff=self.velocity.vy*time)
