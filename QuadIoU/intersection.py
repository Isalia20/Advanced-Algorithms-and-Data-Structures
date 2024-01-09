from shapely import Polygon, Point

# {{{ {20, 200}, {200, 20}, {240, 130}, {150, 400} }}, 
#  {{ {130, 215}, {130, 40}, {220, 300}, {220, 375}}}}
{{{ {20, 200}, {200, 20}, {240, 130}, {150, 400} }}, {{ {130, 40}, {220, 300}, {220, 375}, {130, 215}}}}
a = Polygon([(20, 200), (200, 20), (240, 130), (150, 400)])
b = Polygon([(130, 40), (220, 300), (220, 375), (130, 215)])
print(a.intersection(b).area)

poly_intersect = a.intersection(b)

list(poly_intersect.exterior.coords)

# X IS 130 Y IS 90 
# X IS 130 Y IS 40 
# X IS 142.857 Y IS 77.1429 
# X IS 201.321 Y IS 246.038 
# X IS 181.279 Y IS 306.163 
# X IS 130 Y IS 215 

some_point = Point(130, 40)
a.contains(some_point)
