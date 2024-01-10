#include <iostream>
#include <cuda_runtime.h>

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

__device__ int findMaxQuadCoordinate(Quadrilateral& box, char x_or_y){
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
        return -1;
    }
}

__device__ int orientation(Point p, Point q, Point r) {
    double val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);

    if (abs(val) < 1e-10) return 0;  // colinear
    return (val > 0) ? 1 : 2;  // clock or counterclock wise
}

__device__ int isPointInsideQuadrilateral(Point point_to_check, Quadrilateral box) {
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

__device__ bool onSegment(Point p, Point q, Point r) {
    return q.x <= max(p.x, r.x) && q.x >= min(p.x, r.x) &&
           q.y <= max(p.y, r.y) && q.y >= min(p.y, r.y);
}

// Helper function to check if two given points are equal
__device__ bool arePointsEqual(const Point& p1, const Point& p2) {
    return abs(p1.x - p2.x) < 1e-10 && abs(p1.y - p2.y) < 1e-10;
}

__device__ bool doIntersect(Point p1, Point q1, Point p2, Point q2, Point& intersection) {
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

__device__ int findIntersectionPoints(Quadrilateral quad_1, Quadrilateral quad_2, Point* intersections, int maxIntersections) {
    int numIntersections = 0;
    for (int i = 0; i < 4; ++i) {
        Edge e1 = {quad_1.corners[i], quad_1.corners[(i + 1) % 4]};
        for (int j = 0; j < 4; ++j) {
            Edge e2 = {quad_2.corners[j], quad_2.corners[(j + 1) % 4]};
            Point intersection;
            if (doIntersect(e1.startPoint, e1.endPoint, e2.startPoint, e2.endPoint, intersection)) {
                // Check if this intersection is already in the intersections array
                bool alreadyExists = false;
                for (int k = 0; k < numIntersections; ++k) {
                    if (arePointsEqual(intersections[k], intersection)) {
                        alreadyExists = true;
                        break;
                    }
                }
                if (!alreadyExists && numIntersections < maxIntersections) {
                    intersections[numIntersections++] = intersection;
                }
            }
        }
    }
    return numIntersections;
}

__device__ int findPointInside(Quadrilateral quad_1, Quadrilateral quad_2, Point* inside_points, int maxPoints) {
    int numInsidePoints = 0;

    for (int i = 0; i < 4; i++) {
        if (isPointInsideQuadrilateral(quad_1.corners[i], quad_2) == 1) {
            if (numInsidePoints < maxPoints) {
                inside_points[numInsidePoints++] = quad_1.corners[i];
            }
        }
        if (isPointInsideQuadrilateral(quad_2.corners[i], quad_1) == 1) {
            if (numInsidePoints < maxPoints) {
                inside_points[numInsidePoints++] = quad_2.corners[i];
            }
        }
    }

    return numInsidePoints;
}

__device__ Point findCentroid(const Point* points, int numPoints) {
    Point centroid = {0, 0};
    if (numPoints <= 0) {
        return centroid; // Return the origin if there are no points
    }

    for (int i = 0; i < numPoints; ++i) {
        centroid.x += points[i].x;
        centroid.y += points[i].y;
    }
    centroid.x /= numPoints;
    centroid.y /= numPoints;
    return centroid;
}

__device__ double computeAngle(const Point& centroid, const Point& p) {
    return atan2(p.y - centroid.y, p.x - centroid.x);
}

__device__ bool comparePoints(const Point& p1, const Point& p2, const Point& centroid) {
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

__device__ void sortPointsClockwise(Point* points, int numPoints) {
    // First, find the centroid
    Point centroid = findCentroid(points, numPoints);

    // Perform selection sort on the array of points
    for (int i = 0; i < numPoints - 1; i++) {
        // Find the minimum element in unsorted array
        int minIdx = i;
        for (int j = i + 1; j < numPoints; j++) {
            if (comparePoints(points[j], points[minIdx], centroid)) {
                minIdx = j;
            }
        }
        // Swap the found minimum element with the first element
        Point temp = points[minIdx];
        points[minIdx] = points[i];
        points[i] = temp;
    }
}

__device__ void sortPointsClockwise(Quadrilateral& quad) {
    // Directly use the corners array of the quadrilateral for sorting
    sortPointsClockwise(quad.corners, 4);
}

__device__ double polygonArea(const Point* polygon, int n) {
    double area = 0.0;
    // Calculate the sum for the Shoelace formula
    for (int i = 0; i < n - 1; ++i) {
        area += (polygon[i].x * polygon[i + 1].y - polygon[i + 1].x * polygon[i].y);
    }
    // Add the last term (closing the polygon loop)
    area += (polygon[n - 1].x * polygon[0].y - polygon[0].x * polygon[n - 1].y);
    return fabs(area) / 2.0;
}

__device__ double polygonArea(const Quadrilateral& quad) {
    // Since a quadrilateral always has 4 points, we pass the corners directly
    return polygonArea(quad.corners, 4);
}

__device__ double intersectionArea(Quadrilateral quad0, Quadrilateral quad1) {
    const int MAX_INTERSECTIONS = 8; // 8 intersections max
    const int MAX_TOTAL_POINTS = MAX_INTERSECTIONS * 2; // Intersection + Inside points
    Point intersectionPoints[MAX_INTERSECTIONS];
    Point insidePoints[MAX_INTERSECTIONS];
    Point allPoints[MAX_TOTAL_POINTS];
    int numIntersectionPoints = 0;
    int numInsidePoints = 0;
    int totalPoints = 0;

    // Sort the corners of both quadrilaterals
    sortPointsClockwise(quad0);
    sortPointsClockwise(quad1);

    // Find intersection points
    numIntersectionPoints = findIntersectionPoints(quad0, quad1, intersectionPoints, MAX_INTERSECTIONS);

    // Find points inside the other quadrilateral
    numInsidePoints = findPointInside(quad0, quad1, insidePoints, MAX_INTERSECTIONS);

    // Combine intersection points with inside points
    for (int i = 0; i < numIntersectionPoints; ++i) {
        allPoints[totalPoints++] = intersectionPoints[i];
    }
    for (int i = 0; i < numInsidePoints; ++i) {
        allPoints[totalPoints++] = insidePoints[i];
    }

    // Sort all points to prepare for area calculation
    sortPointsClockwise(allPoints, totalPoints);

    // Calculate the area of the polygon formed by the points
    double intersectArea = polygonArea(allPoints, totalPoints);

    return intersectArea;
}

__device__ double unionArea(Quadrilateral quad0, Quadrilateral quad1, double intersect_area){
    return polygonArea(quad0) + polygonArea(quad1) - intersect_area;
}

__device__ double calculateIoU(double intersect_area, double union_area){
    return intersect_area / union_area;
}

__global__ void computeIoUKernel(Quadrilateral *quads1, Quadrilateral *quads2, double *output, int n1, int n2) {
    int idx1 = blockIdx.x * blockDim.x + threadIdx.x;
    int idx2 = blockIdx.y * blockDim.y + threadIdx.y;

    if (idx1 < n1 && idx2 < n2) {
        int outputIndex = idx1 * n2 + idx2;
        double areaIntersection = intersectionArea(quads1[idx1], quads2[idx2]);
        double areaUnion = unionArea(quads1[idx1], quads2[idx2], areaIntersection);
        output[outputIndex] = calculateIoU(areaIntersection, areaUnion);
    }
}

const int NUM_QUAD0 = 200;
const int NUM_QUAD1 = 300;

int main() {
    cudaError_t err;
    // Allocate memory on the host using Unified Memory (accessible by both CPU and GPU)
    Quadrilateral *quads0, *quads1;
    err = cudaMallocManaged(&quads0, NUM_QUAD0 * sizeof(Quadrilateral));
    if (err != cudaSuccess) {
        std::cerr << "CUDA error (cudaMallocManaged for quads0): " << cudaGetErrorString(err) << std::endl;
        return -1;
    }
    err = cudaMallocManaged(&quads1, NUM_QUAD1 * sizeof(Quadrilateral));
    if (err != cudaSuccess) {
        std::cerr << "CUDA error (cudaMallocManaged for quads1): " << cudaGetErrorString(err) << std::endl;
        cudaFree(quads0); // Clean up previously allocated memory
        return -1;
    }
    cudaMallocManaged(&quads0, NUM_QUAD0 * sizeof(Quadrilateral));
    cudaMallocManaged(&quads1, NUM_QUAD1 * sizeof(Quadrilateral));

    // Initialize the arrays with test quadrilaterals
    for (int i = 0; i < NUM_QUAD0; ++i) {
        quads0[i].corners[0] = {0.0, 0.0};
        quads0[i].corners[1] = {1.0, 0.0};
        quads0[i].corners[2] = {1.0, 1.0};
        quads0[i].corners[3] = {0.0, 1.0};
    }
    for (int i = 0; i < NUM_QUAD1; ++i) {
        quads1[i].corners[0] = {0.0, 0.0};
        quads1[i].corners[1] = {0.5, 0.0};
        quads1[i].corners[2] = {0.5, 0.5};
        quads1[i].corners[3] = {0.0, 0.5};
    }

    // Allocate memory for the output array
    double *d_output;
    size_t outputSize = NUM_QUAD0 * NUM_QUAD1 * sizeof(double);
    err = cudaMallocManaged(&d_output, outputSize);
    if (err != cudaSuccess) {
        std::cerr << "CUDA error (cudaMallocManaged for d_output): " << cudaGetErrorString(err) << std::endl;
        cudaFree(quads0);
        cudaFree(quads1);
        return -1;
    }
    cudaMallocManaged(&d_output, outputSize);

    // Calculate the number of blocks and threads
    dim3 blockSize(16, 16); // You can tune these numbers based on your GPU capabilities
    dim3 gridSize((NUM_QUAD0 + blockSize.x - 1) / blockSize.x, (NUM_QUAD1 + blockSize.y - 1) / blockSize.y);

    // Launch the kernel
    computeIoUKernel<<<gridSize, blockSize>>>(quads0, quads1, d_output, NUM_QUAD0, NUM_QUAD1);
    err = cudaGetLastError();
    if (err != cudaSuccess) {
        std::cerr << "CUDA error (Kernel launch): " << cudaGetErrorString(err) << std::endl;
        cudaFree(quads0);
        cudaFree(quads1);
        cudaFree(d_output);
        return -1;
    }
    // Wait for the kernel to complete
    err = cudaDeviceSynchronize();
    if (err != cudaSuccess) {
        std::cerr << "CUDA error (cudaDeviceSynchronize): " << cudaGetErrorString(err) << std::endl;
        cudaFree(quads0);
        cudaFree(quads1);
        cudaFree(d_output);
        return -1;
    }
    for (int i = 0; i < 600; i++){
        cout << d_output[i] << endl;
    }
    // Free the allocated managed memory
    cudaFree(quads0);
    cudaFree(quads1);
    cudaFree(d_output);

    return 0;
}