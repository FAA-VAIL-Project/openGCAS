#include <iostream>
#include "../include/Raster.h"

int main() {
    const char* t = "/home/quothbonney/forks/openGCAS2/data/n33_w094_1arc_v3.tif";

    Raster& r = Raster::Instance(t);
}