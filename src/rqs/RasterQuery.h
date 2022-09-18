//
// Created by quothbonney on 9/17/22.
//

#ifndef OPENGCAS_RASTERQUERY_H
#define OPENGCAS_RASTERQUERY_H

#include "gdal_priv.h"
#include <vector>
#include <string>

class RasterQuery {
private:
    std::vector<GDALRasterBand> RasterList;

    RasterQuery();

    ///\brief define RasterList attribute with rasters from data/ directory
    std::vector<GDALRasterBand> readDataDir();

public:
    ///\brief Singleton Instance Get
    static RasterQuery& get();


};


#endif //OPENGCAS_RASTERQUERY_H
