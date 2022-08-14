#include <iostream>
#include <vector>
#include "../include/Raster.h"
#include "../include/PolySelect.h"
#include "../include/structs.h"


int main() {
    const char* oklahoma = "../data/n33_w094_1arc_v3.tif";

    Raster okRaster = Raster(oklahoma);

    std::vector<point> vec;

    point point1{0, 1000};
    point point2{0, 0};
    point point3{1000, 0};
    point point4{1000, 10};

    vec.push_back(point1);
    vec.push_back(point2);
    vec.push_back(point3);
    vec.push_back(point4);
    PolySelect pgon = PolySelect(okRaster, vec);

    geoPoint* testing = pgon.getSelection();

    for(int i = 0; i < pgon.area; i++) {
        std::cout << testing[i].x << " " << testing[i].y << " " << testing[i].z << "\n";
    }
}