#include "gdal_priv.h"

#include "../include/Raster.h"
#include "../include/PolySelect.h"
#include "../include/CircSelect.h"
#include <cassert>
#include <vector>
#include <iostream>

int** Raster::readFromBand() {
    int** block;
    // Allocate space for 2d array block and iterate through array
    block = (int**)malloc(sizeof(int*) * xSize * ySize);
    for(int row = 0; row < ySize; row++) {
        block[row] = (int*)malloc(sizeof(int) * xSize);
        // Read line `row` from raster and file block[row]
        poBand->RasterIO(GF_Read,       // Mode (read)
                         0, 0 + row,    // Offset (x, y)
                         xSize, 1,      // Size of buffer (x, y)
                         block[row],    // Location to write buffer
                         xSize, 1,      // Size of block[row] (x, y)
                         GDT_Int32,     // Data type
                         0, 0           // ???
        );
    }
    return block;
}

Raster::Raster(const char* file)  : pszFilename(file) {
    // Init GDAL and define attributes
    GDALAllRegister();

    // GDAL API and band init
    // See https://gdal.org/tutorials/raster_api_tut.html
    poDataset = (GDALDataset*)GDALOpen(pszFilename, GA_ReadOnly);

    poBand = poDataset->GetRasterBand(1);

    xSize = poBand->GetXSize();
    ySize = poBand->GetYSize();

    rasterBandArray = readFromBand();
}


Raster::~Raster() {
    free(rasterBandArray);
}

int** Raster::getArray() {
    return rasterBandArray;
}

double* Raster::getGeoTransform() {
    return adfGeoTransform;
}


geoPoint* Raster::poly(std::vector<point> p) {
    PolySelect s = PolySelect(*this, p);
    return s.getSelection();
}

geoPoint* Raster::circ(int radius, point center) {
    CircSelect s(*this, radius, center);
    return s.getSelection();
}

const char* Raster::getInstanceName() {
    return pszFilename;
}