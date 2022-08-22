#include <iostream>
#include <vector>
#include "../include/Raster.h"
#include "../include/PolySelect.h"
#include "../include/structs.h"


int main(int argc, char *argv[]) {
    const char* oklahoma = "../data/gabon.tif";
    if(argc == 2) {
        oklahoma = argv[1];
    }
    std::cout << "Reading: " << oklahoma << std::endl;

    Raster okRaster = Raster(oklahoma);

    std::vector<point> vec;

    point point1{10000, 10000};
    point point2{10010, 10010};
    point point3{10000, 10010};
    point point4{10010, 10000};

    vec.push_back(point1);
    vec.push_back(point2);
    vec.push_back(point3);
    vec.push_back(point4);

    geoPoint* testing = okRaster.poly(vec);

    for(int i = 0; i < 1000; i++)
        std::cout << testing[i].z << "\n";

    delete[] testing;
}
