#include "quad_iou.cpp"

void printQuadrilateral(const Quadrilateral& quad) {
    for (int i = 0; i < 4; ++i) {
        std::cout << "Corner " << i+1 << ": (" << quad.corners[i].x << ", " << quad.corners[i].y << ")\n";
    }
}
