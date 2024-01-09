import random
from shapely import Polygon


Polygon()

def generate_random_point(x_min, x_max, y_min, y_max):
    x = random.uniform(x_min, x_max)
    y = random.uniform(y_min, y_max)
    return (x, y)

def cross_product(p1, p2, p3):
    """
    Calculate the cross product of vectors p1p2 and p2p3.
    We only need the sign of the Z component of the cross product to check for convexity.
    """
    x1, y1 = p2[0] - p1[0], p2[1] - p1[1]
    x2, y2 = p3[0] - p2[0], p3[1] - p2[1]
    return x1 * y2 - x2 * y1

def is_convex_quad(points):
    """
    Check if a quadrilateral is convex based on the cross product of edges.
    
    Args:
    points -- a list of 4 points, where each point is a tuple (x, y)
    
    Returns:
    True if the quadrilateral is convex, False otherwise.
    """
    if len(points) != 4:
        raise ValueError("The function expects exactly four points.")

    # Calculate cross products for consecutive edges
    cp = []
    for i in range(4):
        cp.append(cross_product(points[i], points[(i + 1) % 4], points[(i + 2) % 4]))
    
    # Check that all cross products have the same sign
    # This checks if all turns are in the same direction
    return all(x > 0 for x in cp) or all(x < 0 for x in cp)

def sort_points_counter_clockwise(points):
    # Sort the points in counterclockwise order
    # A simple method is to compute the centroid and sort by angle relative to the centroid
    # For simplicity, this function assumes the points are already in counterclockwise order
    return points

def generate_convex_quadrilateral():
    x_min, x_max, y_min, y_max = 0, 100, 0, 100  # Define bounds
    points = [generate_random_point(x_min, x_max, y_min, y_max) for _ in range(4)]
    sorted_points = sort_points_counter_clockwise(points)
    
    while not is_convex_quad(sorted_points):
        points = [generate_random_point(x_min, x_max, y_min, y_max) for _ in range(4)]
        sorted_points = sort_points_counter_clockwise(points)
    
    return sorted_points

# Generate a convex quadrilateral
for i in range(1000):
    quad = generate_convex_quadrilateral()
    print(quad)
