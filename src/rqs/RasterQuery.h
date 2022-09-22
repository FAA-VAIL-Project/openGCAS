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

    RasterQuery() = default;



    struct geoTransformData {
        int index;
        std::string fname;
        double lon_o;
        double lon_res;
        double lat_o;
        double lat_res;
        int r_xSize;
        int r_ySize;
    };

protected:
    std::vector<geoTransformData> dataDirTransform;

public:
    ///\brief Singleton Instance Get
    static RasterQuery& get();

    ///\brief define RasterList attribute with rasters from data/ directory
    void readDataDir();

};


#endif //OPENGCAS_RASTERQUERY_H
