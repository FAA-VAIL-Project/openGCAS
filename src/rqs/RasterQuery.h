//
// Created by Jack D.V. Carson on 9/17/2022.
// Copyright (C) GNU LESSER GENERAL PUBLIC LICENSE
//

#ifndef OPENGCAS_RASTERQUERY_H
#define OPENGCAS_RASTERQUERY_H

#include "gdal_priv.h"
#include "../../include/structs.h"
#include <vector>
#include <iostream>
#include <algorithm>
#include <string>
#include <memory>
#include <cmath>
#include <experimental/filesystem>

class rqsDataBlock;

/**
 * @brief Raster Query System for data access and raster interface
 * @see RasterQuery& rq = RasterQuery::get()
 *
 * RasterQuery Singleton represents data access level 0 (see docs/data_access.md).
 * It is the base class for all large memory allocations.
 */
class RasterQuery {
private:
    /**
     * @brief Struct containing information about each raster
     * @see [0] index, [1] fname, [2] lon_o, [3] lon_res, [4] lat_o, [5] lat_res, [6] r_xSize, [7], r_ySize
     */
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

    /**
     * @brief RasterQuery Private Singleton Constructor
     */
    RasterQuery();

    /**
     * @brief define m_dataDirTransform vector attribute with
     * geoTransForm data from data/ directory
     */
    static auto readDataDir() -> std::vector<geoTransformData>;

    friend class rqsDataBlock;

protected:
    // Vector of data/ geoTransformData
    std::vector<geoTransformData> m_dataDirTransform;

    //Vector of open RasterBands based on geospatial position
    std::vector<int> m_rasterCallOrder;

public:
    /**
     * @brief Singleton Instance method
     */
    static RasterQuery& get();

    void init(llPoint llLocation);

    /**@brief Convert llPoint into discrete nPoint on a raster
     *
     * @param llPoint to convert
     * @return nPoint of closest raster index
     */
    auto discreteIndex(llPoint workingPoint) -> nPoint;

    /**
     * @brief Define a list of Raster files from which data might be realistically found in a
     * 3x3 grid.
     * @param llLocation is llPoint of current location
     */
    void defineCallOrder(llPoint llLocation);
};

class rqsDataBlock {
private:
    /*
     * Block data is stored in a 2d smart pointer _spBlock which consists of typedef
     * Smart pointers _spRow. The memory  is allocated in the private init() method
     * which is called in the constructor
     */
    typedef std::unique_ptr<int[]> _spRow;

    /**
     * @brief Smart pointer block data container
     */
    std::unique_ptr<_spRow[]> _spBlock;

    /**
     * @brief Initialize memory block of size 1024x1024 to _spBlock
     */
    void init();

public:
    const int m_id;

    /**
     * Basic constructor calling init memory functions of rqsDataBlock
     * @param int id for memory alloc
     */
    explicit rqsDataBlock(int id, RasterQuery& rq);
};


#endif //OPENGCAS_RASTERQUERY_H
