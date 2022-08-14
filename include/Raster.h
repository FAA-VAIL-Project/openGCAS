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
    const char* _filepath;
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

    // Constructor takes a string for filename
    // Use relative path
    Raster(const char* file);

    // Frees int** array
    ~Raster();

    static Raster r_instance;

public:
    int xSize, ySize;

    // Singleton instance of the class
    // Call with Raster::Instance(const char* file)
    // Guarantees uniqueness
    static Raster& Instance(const char* file);

    // Returns rasterBandArray
    int** getArray();

    // Returns adfGeoTransform
    double* getGeoTransform();

    geoPoint* poly(std::vector<point> p);
};


#endif //OPENGCAS_RASTER_H
