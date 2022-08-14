#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm> // for std::sort

#include "../include/Raster.h"

struct point {
    int x;
    int y;
};


class PolySelect {
private:
    Raster* raster;
    std::vector<point> points;

    // Operator predicate passed to std::sort
    // Calculates the theta in the polar coordinate of point& input
    // and sorts based on theta without creating a new vector in memory
    struct sort {
        point center;
        bool operator()(point const& lhs, point const& rhs) {
            float lhsTheta = atan2(lhs.y - center.y, lhs.x - center.x);
            float rhsTheta = atan2(rhs.y - center.y, rhs.x - center.x);

            return lhsTheta < rhsTheta;
        }
    };


    // Ensures that the points are within the array (prevents memory violations)
    // TODO: Allow for invalid indexes and just create 0's
    void isIndexable() {
        for (const auto &p: points) {
            try {
                if (p.x > raster->xSize || p.x < 0 || p.y > raster->ySize || p.y < 0) {
                    throw 100;
                }
            }
            catch (int e) {
                std::cerr << "Cannot create PolySelect! Point " << p.x << " " << p.y << " is outside raster range "
                          << raster->xSize << " " << raster->ySize << "...";
                exit(1);
            }
        }
    }

    // Define center point for polar coords
    point defineCenter() {
        int workingX = 0, workingY = 0;

        for( const auto &p : points) {
            workingX += p.x;
            workingY += p.y;
        }
        point c;
        c.x = workingX / 4;
        c.y = workingY / 4;
        return c;
    }


public:
    PolySelect(Raster &r, std::vector<point> pointVec) {
        this->points = pointVec;
        this->raster = &r;

        // Check that all points within raster
        isIndexable();

        // Init object referenced in std::sort
        sort polarVectorSort{defineCenter()};

        std::sort(points.begin(), points.end(), polarVectorSort);

        for(const point i : points) {
            std::cout << i.x << " " << i.y << "\n";
        }
    }
};


int main() {
    const char* oklahoma = "../gtif/n33_w094_1arc_v3.tif";

    Raster okRaster = Raster(oklahoma);

    std::vector<point> vec;

    point point1{10, 23};
    point point2{18, 43};
    point point3{24, 28};
    point point4{56, 2};
    point point5{46, 99};

    vec.push_back(point1);
    vec.push_back(point2);
    vec.push_back(point3);
    vec.push_back(point4);
    vec.push_back(point5);
    PolySelect pgon = PolySelect(okRaster, vec);
}