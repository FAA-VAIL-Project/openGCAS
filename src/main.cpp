#include <iostream>
#include "gdal_priv.h" // Testing Linker
#include "cpl_conv.h"
#include "Raster.h"


int main() {
    const char* oklahoma = "../n33_w094_1arc_v3.tif";

    Raster okRaster = Raster(oklahoma);

    int** ras = okRaster.getArray();
    for(int i = 0; i < 20; i++) {
        for(int j = 0; j < 20; j++) {
            std::cout << ras[i][j] << " ";
        }
        std::cout << "\n";
    }
}