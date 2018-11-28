from roadmap import RoadMap
from geogebra import Geogebra
import time

begin = time.time()
global count
count = 0

print("GEOGEBRA")
geo = Geogebra('../robots/bornibus.ggb')
print("FIN GEOGEBRA ", time.time() - begin)
print("ROADMAP ", time.time() - begin)
rm = RoadMap.load(geo)
print("Fin Chargement ", time.time() - begin)

