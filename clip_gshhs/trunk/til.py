import math
def num2deg(xtile, ytile, zoom):
   n = 2.0 ** zoom
   lon_deg = xtile / n * 360.0 - 180.0
   lat_rad = math.atan(math.sinh(math.pi * (1 - 2 * ytile / n)))
   lat_deg = math.degrees(lat_rad)
   return(lat_deg, lon_deg)

(lat_deg, lon_deg)=num2deg(0, 1, 3)

print lat_deg
print lon_deg


