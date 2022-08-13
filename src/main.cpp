#include <iostream>
#include <stack>
#include <vector>

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


    // Probably a much better way to do this
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

    point defineCenter() {
        int workingX = 0, workingY = 0;

        for( const auto &p : points) {
            workingX += p.x;
            workingY += p.y;
        }
        point c;
        c.x = workingX / 2;
        c.y = workingY / 2;
        return c;
    }

public:
    PolySelect(Raster &r, std::vector<point> pointVec) {
        this->points = pointVec;
        this->raster = &r;

        isIndexable();

        center = defineCenter();
        std::cout << center.x << "\n" << center.y;
    }
};


int main() {
    const char* oklahoma = "../gtif/n33_w094_1arc_v3.tif";

    Raster okRaster = Raster(oklahoma);

    std::vector<point> vec;
    point point1, point2;
    point1.x = 0; point1.y = 0;
    point2.x = 10; point2.y = 10;
    vec.push_back(point1);

    PolySelect pgon = PolySelect(okRaster, vec);
}