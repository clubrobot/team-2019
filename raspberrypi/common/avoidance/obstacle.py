class Velocity:
    def __init__(self, vx, vy):
        self.vx = vx
        self.vy = vy

class Obstacle:
    def __init__(self, geom, velocity):
        self.geom = geom
        self.velocity = velocity

    def move(self, time):
        self.geom.translate(self.velocity.vx*time, self.velocity.vy*time)
