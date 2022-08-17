#include <iostream>
#include "../include/Raster.h"

int main() {
    const char* t = "../../data/n33_w094_1arc_v3.tif";

    Raster& r = Raster::Instance(t);
}