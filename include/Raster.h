//
// Core class for getting array from .tif file
//

#ifndef OPENGCAS_RASTER_H
#define OPENGCAS_RASTER_H

#include "gdal_priv.h"
#include "structs.h"
#include <vector>

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

    int** rasterBandArray;

    int** readFromBand();

public:
    int xSize, ySize;

    // Constructor takes a string for filename
    // Use relative path
    Raster(const char* file);
    Raster();

    // Frees int** array
    ~Raster();

    // Returns rasterBandArray
    int** getArray();

    // Returns adfGeoTransform
    double* getGeoTransform();

    // Get polygon selection from points
    geoPoint* poly(std::vector<point> p);

    // Get circle selection
    geoPoint* circ(int radius, point center);

    // For testing singleton property
    const char* getInstanceName();
};


#endif //OPENGCAS_RASTER_H
