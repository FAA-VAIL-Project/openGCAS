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


    //geoPoint* p = okRaster.poly(ps);

    geoPoint* testing2 = okRaster.circ(5, point{10, 10});



    return 0;
}
