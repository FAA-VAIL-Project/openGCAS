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
    point center{};

    // Defined private within class to avoid unnecessary global structs
    struct polarPoints {
        double r;
        double theta;
    };

    // Operator predicate passed to std::sort to sort the polarPoint struct for theta
    struct {
        bool operator()(polarPoints const& lhs, polarPoints const& rhs) {
            return lhs.theta < rhs.theta;
        }
    } polarVectorSort;


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



    std::vector<polarPoints> polarVector() {
        std::vector<polarPoints> pVec;
        // Polar coordinate conversion
        // Subtracts center because not centered at Origin
        for(const point& i : points) {
            polarPoints nPolar;
            nPolar.theta = atan2(i.y - center.y, i.x - center.x);
            nPolar.r = pow((i.x - center.x), 2) + pow((i.y - center.y), 2);
            pVec.push_back(nPolar);
        }
        return pVec;
    }



public:
    PolySelect(Raster &r, std::vector<point> pointVec) {
        this->points = pointVec;
        this->raster = &r;

        isIndexable();

        center = defineCenter();

        auto pVec = polarVector();

        std::cout << center.x << " " << center.y << "\n";

        std::sort(pVec.begin(), pVec.end(), polarVectorSort);
        for(const auto i : pVec) {
            std::cout << i.r << " " << i.theta << std::endl;
        }
    }
};


int main() {
    const char* oklahoma = "../gtif/n33_w094_1arc_v3.tif";

    Raster okRaster = Raster(oklahoma);

    std::vector<point> vec;
    point point1, point2, point3, point4;
    point1.x = 0; point1.y = 0;
    point2.x = 80; point2.y = 40;
    point3.x = 9; point3.y = 2;
    point4.x = 18; point4.y = 5;

    vec.push_back(point1);
    vec.push_back(point2);
    vec.push_back(point3);
    vec.push_back(point4);
    PolySelect pgon = PolySelect(okRaster, vec);
}