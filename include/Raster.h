//
// Core class for getting array from .tif file
//

#ifndef OPENGCAS_RASTER_H
#define OPENGCAS_RASTER_H

#include "gdal_priv.h"

class Raster {
private:
    const char* pszFilename;
    GDALRasterBand* poBand;
    GDALDataset* poDataset;

    /* FROM DOCS
    adfGeoTransform[0] top left x
    adfGeoTransform[1] w-e pixel resolution
    adfGeoTransform[2] 0
    adfGeoTransform[3] top left y
    adfGeoTransform[4] 0
    adfGeoTransform[5] n-s pixel resolution (negative value)
    */
    double adfGeoTransform[6];

    int xSize, ySize;
    int** rasterBandArray;

    int** readFromBand();

public:
    Raster(const char* file);
    ~Raster();

    int** getArray();
};


#endif //OPENGCAS_RASTER_H
