#include <iostream>
#include <vector>
#include "../include/Raster.h"
#include "../include/PolySelect.h"
#include "../include/structs.h"
#include "../include/CircSelect.h"


int main(int argc, char *argv[]) {
    const char* oklahoma = "../data/gabon.tif";
    if(argc == 2) {
        oklahoma = argv[1];
    }

    Raster okRaster = Raster(oklahoma);
    std::vector<point> ps = {
            {2,4},
            {3, 9},
            {12, 94},
            {18, 3}
    };
    geoPoint* p = okRaster.poly(ps);

    CircSelect circtest = CircSelect(okRaster, 5, point{400, 200});


    return 0;
}
