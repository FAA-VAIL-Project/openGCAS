//
// Created by quothbonney on 9/17/22.
//

#ifndef OPENGCAS_RASTERQUERY_H
#define OPENGCAS_RASTERQUERY_H

#include "gdal_priv.h"
#include "../../include/structs.h"
#include <vector>
#include <string>

class RasterQuery {
private:
    std::vector<GDALRasterBand> RasterCallOrder;

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

    ///\brief RasterQuery Private Singleton Constructor
    RasterQuery();

    ///\brief define RasterList attribute with rasters from data/ directory
    auto readDataDir() -> std::vector<geoTransformData>;

protected:
    std::vector<geoTransformData> dataDirTransform;

public:
    ///\brief Singleton Instance Get
    static RasterQuery& get();

    ///\brief Convert llPoint into discrete nPoint on a raster
    auto discreteIndex(llPoint workingPoint) -> nPoint;

};


#endif //OPENGCAS_RASTERQUERY_H
