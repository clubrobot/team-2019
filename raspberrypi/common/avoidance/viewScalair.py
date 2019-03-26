import common.geogebra as geogebra
from common.avoidance.obstacles import *
from common.avoidance.funct import *
import common.avoidance.geometry as geometry

from mpl_toolkits.mplot3d import Axes3D
import matplotlib.pyplot as plt
from matplotlib import cm
import numpy as np

# Extract geogebra data
#all
geo = geogebra.Geogebra("test.ggb")
robot = geometry.Point(*geo.get("origin"))
goal = geometry.Point(*geo.get("goal"))
period = 1
max_w_pf = 10
robot_width = 300

objs = []

funct = funct_list["lin"](a=max_w_pf/robot_width, b=max_w_pf)
for obs in geo.getall("obsc_*"):
    print("c")
    objs += [DiskObs(Point(obs[0], obs[1]), obs[2], funct)]
poly_in = geo.getall("obsin_*")
for poly in poly_in:
    print("p")
    objs += [PolygonObs(poly, funct)]

#pf
maps = geometry.Point(*geo.get("MAP"))
objs += [Map(maps.x, maps.y, funct)]

print(objs)

fig = plt.figure()
ax = fig.gca(projection='3d')
# Make data.
X = np.arange(0, 2000, 10, dtype =np.float32)
Y = np.arange(0, 3000, 10, dtype =np.float32)
X, Y = np.meshgrid(X, Y)
Z = np.zeros_like(X, dtype=np.float32)
# Create obj
for i in range(X.shape[0]):
    for k in range(X.shape[1]):
        for obj in objs:
            Z[i, k] += obj.get_scalar([float(X[i, k]), float(Y[i, k])])

surf = ax.plot_surface(X, Y, Z, cmap=cm.coolwarm,
                       linewidth=0, antialiased=False)
plt.show()