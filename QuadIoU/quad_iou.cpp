#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <chrono>  // For timing

using namespace std;

struct Point {
    double x, y;
};

struct Edge {
    struct Point startPoint;
    struct Point endPoint;
};

struct Quadrilateral {
    Point corners[4];
};

// Helper function to calculate the cross product of two points
double crossProduct(Point a, Point b) {
    return a.x * b.y - b.x * a.y;
}

int findMaxQuadCoordinate(Quadrilateral& box, char x_or_y){
    // Find the maximum x-coordinate of the quadrilateral
    if (x_or_y == 'x'){
        int max_x = box.corners[0].x;
        for (int i = 1; i < 4; ++i) {
            if (box.corners[i].x > max_x) {
                max_x = box.corners[i].x;
            }
        }
        return max_x;
    } else if (x_or_y == 'y'){
        int max_y = box.corners[0].y;
        for (int i = 1; i < 4; ++i) {
            if (box.corners[i].y > max_y) {
                max_y = box.corners[i].y;
            }
        }
        return max_y;
    } else {
        return -1; //TODO maybe raise error here?
    }
}

// Helper function to find orientation of ordered triplet (p, q, r).
// The function returns following values
// 0 --> p, q and r are colinear
// 1 --> Clockwise
// 2 --> Counterclockwise
int orientation(Point p, Point q, Point r) {
    double val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);

    if (abs(val) < 1e-10) return 0;  // colinear
    return (val > 0) ? 1 : 2;  // clock or counterclock wise
}

int isPointInsideQuadrilateral(Point point_to_check, Quadrilateral box) {
    // Find the maximum x-coordinate of the quadrilateral
    int max_x = findMaxQuadCoordinate(box, 'x');
    int max_y = findMaxQuadCoordinate(box, 'y');
    // If the point's x-coordinate is greater than the max x-coordinate, it's outside
    if (point_to_check.x > max_x) return -1;
    if (point_to_check.y > max_y) return -1;

    // For each edge of the quadrilateral
    for (int i = 0; i < 4; i++) {
        // Get the current edge's start and end points
        Point start_point = box.corners[i];
        Point end_point = box.corners[(i + 1) % 4]; // Wrap around to the first point after the last
        // Calculate the cross product to determine where the point is in relation to the edge
        double cross_product = (start_point.y - point_to_check.y) * (end_point.x - point_to_check.x) -
                                (start_point.x - point_to_check.x) * (end_point.y - point_to_check.y);
        if (cross_product > 0) {
            return -1; // Point is outside the quadrilateral
        }
        else if (cross_product == 0) {
            return 0; // Point is on the boundary of the quadrilateral
        }
    }
    return 1; // Point is inside the quadrilateral
}

bool onSegment(Point p, Point q, Point r) {
    return q.x <= max(p.x, r.x) && q.x >= min(p.x, r.x) &&
           q.y <= max(p.y, r.y) && q.y >= min(p.y, r.y);
}

// Helper function to check if two given points are equal
bool arePointsEqual(const Point& p1, const Point& p2) {
    return abs(p1.x - p2.x) < 1e-10 && abs(p1.y - p2.y) < 1e-10;
}

bool doIntersect(Point p1, Point q1, Point p2, Point q2, Point& intersection) {
    // Find the four orientations needed for general and
    // special cases
    int o1 = orientation(p1, q1, p2);
    int o2 = orientation(p1, q1, q2);
    int o3 = orientation(p2, q2, p1);
    int o4 = orientation(p2, q2, q1);

    // General case
    if (o1 != o2 && o3 != o4) {
        // Line AB represented as a1x + b1y = c1
        double a1 = q1.y - p1.y;
        double b1 = p1.x - q1.x;
        double c1 = a1 * (p1.x) + b1 * (p1.y);

        // Line CD represented as a2x + b2y = c2
        double a2 = q2.y - p2.y;
        double b2 = p2.x - q2.x;
        double c2 = a2 * (p2.x) + b2 * (p2.y);

        double determinant = a1 * b2 - a2 * b1;

        if (abs(determinant) < 1e-10) {
            return false; // The lines are parallel. This is simplified
                          // by returning false here.
        } else {
            intersection.x = (b2 * c1 - b1 * c2) / determinant;
            intersection.y = (a1 * c2 - a2 * c1) / determinant;
            return true;
        }
    }

    // Special Cases
    // p1, q1 and p2 are colinear and p2 lies on segment p1q1
    if (o1 == 0 && onSegment(p1, p2, q1)) {
        intersection = p2;
        return true;
    }

    // p1, q1 and q2 are colinear and q2 lies on segment p1q1
    if (o2 == 0 && onSegment(p1, q2, q1)) {
        intersection = q2;
        return true;
    }

    // p2, q2 and p1 are colinear and p1 lies on segment p2q2
    if (o3 == 0 && onSegment(p2, p1, q2)) {
        intersection = p1;
        return true;
    }

    // p2, q2 and q1 are colinear and q1 lies on segment p2q2
    if (o4 == 0 && onSegment(p2, q1, q2)) {
        intersection = q1;
        return true;
    }

    return false; // Doesn't fall in any of the above cases
}

vector<Point> findIntersectionPoints(Quadrilateral quad_1, Quadrilateral quad_2) {
    vector<Point> intersections;
    
    for (int i = 0; i < 4; ++i) {
        Edge e1 = {quad_1.corners[i], quad_1.corners[(i + 1) % 4]};
        for (int j = 0; j < 4; ++j) {
            Edge e2 = {quad_2.corners[j], quad_2.corners[(j + 1) % 4]};
            Point intersection;
            if (doIntersect(e1.startPoint, e1.endPoint, e2.startPoint, e2.endPoint, intersection)) {
                // Check if this intersection is already in the intersections vector
                if (find_if(intersections.begin(), intersections.end(),
                    [&intersection](const Point& p) {
                        return arePointsEqual(p, intersection);
                    }) == intersections.end()) {
                    intersections.push_back(intersection);
                }
            }
        }
    }
    return intersections;
}

vector<Point> findPointInside(Quadrilateral quad_1, Quadrilateral quad_2){
    vector<Point> inside_points;

    for (int i = 0; i < 4; i++){
        if (isPointInsideQuadrilateral(quad_1.corners[i], quad_2) == 1) inside_points.push_back(quad_1.corners[i]);
        if (isPointInsideQuadrilateral(quad_2.corners[i], quad_1) == 1) inside_points.push_back(quad_2.corners[i]);
    }
    return inside_points;
}

// Function to compute the centroid of a vector of points
Point findCentroid(const std::vector<Point>& points) {
    Point centroid = {0, 0};
    for (const Point& p : points) {
        centroid.x += p.x;
        centroid.y += p.y;
    }
    centroid.x /= points.size();
    centroid.y /= points.size();
    return centroid;
}

// Function to compute the angle between two points with respect to the horizontal axis
double computeAngle(const Point& centroid, const Point& p) {
    return atan2(p.y - centroid.y, p.x - centroid.x);
}

// Comparator function to sort points clockwise
bool comparePoints(const Point& p1, const Point& p2, const Point& centroid) {
    double angle1 = computeAngle(centroid, p1);
    double angle2 = computeAngle(centroid, p2);

    // If angles are the same, sort based on distance from centroid
    if (angle1 == angle2) {
        double dist1 = (p1.x - centroid.x) * (p1.x - centroid.x) +
                       (p1.y - centroid.y) * (p1.y - centroid.y);
        double dist2 = (p2.x - centroid.x) * (p2.x - centroid.x) +
                       (p2.y - centroid.y) * (p2.y - centroid.y);
        return dist1 < dist2;
    }
    return angle1 < angle2;
}

// Sorts a vector of points in clockwise order
void sortPointsClockwise(std::vector<Point>& points) {
    Point centroid = findCentroid(points);
    std::sort(points.begin(), points.end(),
              [&centroid](const Point& p1, const Point& p2) {
                  return comparePoints(p1, p2, centroid);
              });
}

// Sorts the points of a Quadrilateral in clockwise order
void sortPointsClockwise(Quadrilateral& quad) {
    std::vector<Point> points(quad.corners, quad.corners + 4);
    sortPointsClockwise(points);
    std::copy(points.begin(), points.end(), quad.corners);
}

double polygonArea(vector<Point>& polygon) {
    double area = 0.0;
    int n = polygon.size();
    // Calculate the sum for the Shoelace formula
    for (int i = 0; i < n - 1; ++i) {
        area += (polygon[i].x * polygon[i + 1].y - polygon[i + 1].x * polygon[i].y);
    }
    // Add the last term (closing the polygon loop)
    area += (polygon[n - 1].x * polygon[0].y - polygon[0].x * polygon[n - 1].y);
    return std::abs(area) / 2.0;
}

double polygonArea(const Quadrilateral& quad) {
    std::vector<Point> points(quad.corners, quad.corners + 4);
    return polygonArea(points);
}

double intersectionArea(Quadrilateral quad0, Quadrilateral quad1){
    vector<Point> polygon_points;

    sortPointsClockwise(quad0);
    sortPointsClockwise(quad1);
    polygon_points = findIntersectionPoints(quad0, quad1);
    vector<Point> inside_points = findPointInside(quad0, quad1);
    // Extend polygon points with inside points
    polygon_points.insert(polygon_points.end(), inside_points.begin(), inside_points.end());
    sortPointsClockwise(polygon_points);
    double intersect_area = polygonArea(polygon_points);
    return intersect_area;
}

double unionArea(Quadrilateral quad0, Quadrilateral quad1, double intersect_area){
    return polygonArea(quad0) + polygonArea(quad1) - intersect_area;
}

double calculateIoU(double intersect_area, double union_area){
    return intersect_area / union_area;
}
