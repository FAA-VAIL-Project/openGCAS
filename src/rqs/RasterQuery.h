//
// Created by Jackie D.V. Carson on 9/17/2022.
// Copyright (C) GNU LESSER GENERAL PUBLIC LICENSE
//

#ifndef OPENGCAS_RASTERQUERY_H
#define OPENGCAS_RASTERQUERY_H

#include "gdal_priv.h"
#include "../../include/structs.h"
#include <vector>
#include <iostream>
#include <string>
#include <memory>

/**
 * @brief Raster Query System for data access and raster interface
 * @see RasterQuery& rq = RasterQuery::get()
 *
 * RasterQuery Singleton represents data access level 0 (see docs/data_access.md).
 * It is the base class for all large memory allocations.
 */
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

class rqsDataBlock {
private:
    typedef std::unique_ptr<int[]> _spRow;

    std::unique_ptr<_spRow[]> _spBlock;

public:
    const int m_id;

    ///\brief Default constructor with initializer list
    rqsDataBlock(int id);

    ///\brief Initialize memory block of size 1024x1024
    void init();
};


#endif //OPENGCAS_RASTERQUERY_H
