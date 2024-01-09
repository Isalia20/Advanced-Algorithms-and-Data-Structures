#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

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

struct Polygon {
    // Maximum amount of corners our polygon can have is
    // 8, 2 quadrilaterals can have maximum of 8 intersections
    // due to geometric constraints
    Point corners[8];
};

double findMaxQuadCoordinate(Quadrilateral& box, char x_or_y){
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

int isPointInsideQuadrilateral(Point point_to_check, Quadrilateral box) {
    // Find the maximum x-coordinate of the quadrilateral
    double max_x = findMaxQuadCoordinate(box, 'x');
    double max_y = findMaxQuadCoordinate(box, 'y');
    // If the point's x-coordinate is greater than the max x-coordinate, it's outside
    if (point_to_check.x > max_x) return -1;
    if (point_to_check.y > max_y) return -1;

    // For each edge of the quadrilateral
    for (int i = 0; i < 4; i++) {
        // Get the current edge's start and end points
        Point start_point = box.corners[i];
        Point end_point = box.corners[(i + 1) % 4]; // Wrap around to the first point after the last
        // Calculate the cross product to determine where the point is in relation to the edge
        double cross_product = 

                            (start_point.y - point_to_check.y) * (end_point.x - point_to_check.x) -
                                (start_point.x - point_to_check.x) * (end_point.y - point_to_check.y);
                            // (start_point.y - point_to_check.y) * (end_point.x - point_to_check.x) -
                            // (start_point.x - point_to_check.x) * (end_point.y - start_point.y); // ORIGINAL ONE
                            (start_point.y - point_to_check.y) * (end_point.x - point_to_check.x) -
                                (start_point.x - point_to_check.x) * (end_point.y - point_to_check.y); // GPT WITH TIP
        if (cross_product > 0) {
            return -1; // Point is outside the quadrilateral
        }
        else if (cross_product == 0) {
            return 0; // Point is on the boundary of the quadrilateral
        }
    }
    return 1; // Point is inside the quadrilateral
}

int main(){
    // test();
    Quadrilateral quad0{{ {20, 200}, {200, 20}, {240, 130}, {150, 400} }};
    Point some_point;
    some_point.x = 130;
    some_point.y = 40;
    cout << isPointInsideQuadrilateral(some_point, quad0) << endl; 
    return 0;
}
