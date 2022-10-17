#include <catch2/catch.hpp>
#include "../include/Raster.h"
#include <vector>
#include <ctime>
#include <algorithm>
/*
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
        }
    }
    return false;
}

TEST_CASE("Raster Test") {
    const char *ok = "../../data/oklahoma.tif";
    const char *ga = "../../data/gabon.tif";

    // Test default constructor
    Raster r = Raster(ok);
    Raster g = Raster(ga);

    SECTION("Initialize Raster Tests") {
        // Ensure that each raster is unique
        CHECK(r.getArray() != nullptr);
        CHECK(r.getArray() != g.getArray());
        CHECK(r.getInstanceName() != g.getInstanceName());
    }

    SECTION("PolySelect Non-Intersection") {
        std::srand(std::time(nullptr)); // Init seed
        int maxSize = std::min(r.xSize, r.ySize);
        int vectorSize = std::rand() % 50;
        for(int j = 0; j < 10; ++j) {
            std::vector<point> vec;
            for (int i = 0; i < vectorSize; ++i) {
                int randX = std::rand() % maxSize;
                int randY = std::rand() % maxSize;
                vec.push_back(point{randX, randY});
            }
            CHECK(intersections(vec) == 1);
        }
    }
}
*/