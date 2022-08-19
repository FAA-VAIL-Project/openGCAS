//#define CATCH_CONFIG_MAIN
//#include <catch2/catch.hpp>
#include "../include/Raster.h"
#include <iostream>
#include <algorithm>
#include <vector>

int orientation(point p, point q, point r) {
    int val = (q.y - p.y) * (r.x - q.x) -
            (q.x - p.x) * (r.y - q.y);

    if (val == 0) return 0;
    return (val > 0) ? 1 : 2;
}

bool onSegment(point p, point q, point r) {
    if (q.x <= std::max(p.x, r.x) && q.x >= std::min(p.x, r.x) &&
        q.y <= std::max(p.y, r.y) && q.y >= std::min(p.y, r.y))
       return true;

    return false;
}

bool intersections(std::vector<point> p) {
    for(int i = 0; i < p.size(); ++i) {
        // Create line segment P1Q1 between p1 and its neighbor
        const point p1 = p[i];
        const point q1 = p[(i + 1) % p.size()];

        // Get the next two non-connected elements
        for (int j = 0; j < p.size() - 3; ++j) {
            const point p2 = p[(i + j + 2) % p.size()];
            const point q2 = p[(i + j + 3) % p.size()];

            int o1 = orientation(p1, q1, p2);
            int o2 = orientation(p1, q1, q2);
            int o3 = orientation(p2, q2, p1);
            int o4 = orientation(p2, q2, q1);

            if (o1 != o2 && o3 != o4)
                return true;

            // Special Cases
            if (o1 == 0 && onSegment(p1, p2, q1)) return true;
            if (o2 == 0 && onSegment(p1, q2, q1)) return true;
            if (o3 == 0 && onSegment(p2, p1, q2)) return true;
            if (o4 == 0 && onSegment(p2, q1, q2)) return true;
            /*
            print testing;
            std::cout << p1.x << " " << p1.y << "   " << q1.x << " " << q1.y << "\n";
            std::cout << p2.x << " " << p2.y << "   " << q2.x << " " << q2.y << "\n\n\n";*/
        }
        // General case

    }
    return false; // Doesn't fall in any of the above cases
}

int main() {
    std::vector<point> p = {
            {1, 1},
            {4, 4},
            {9, 3},
            {6, 6},
    };
    bool b = intersections(p);
    std::cout << b;
}