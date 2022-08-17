#include <iostream>
#include <vector>
#include "../include/Raster.h"


int main(int argc, char *argv[]) {
    const char* oklahoma = "./data/n33_w094_1arc_v3.tif";
    if(argc == 2) {
      oklahoma = argv[1];
    }
    std::cout << "Reading: " << oklahoma << std::endl;


    Raster& okRaster = Raster::Instance(oklahoma);
    Raster& oRaster = Raster::Instance(oklahoma);
/*
    std::vector<point> vec;

    point point1{0, 1000};
    point point2{0, 0};
    point point3{1000, 0};
    point point4{1000, 10};

    vec.push_back(point1);
    vec.push_back(point2);
    vec.push_back(point3);
    vec.push_back(point4);

    geoPoint* testing = okRaster.poly(vec);
    geoPoint* test2 = okRaster.poly(vec);


    for(int i = 0; i < 1000; i++)
        std::cout << testing[i].z << "\n";

    delete[] testing;*/
}
