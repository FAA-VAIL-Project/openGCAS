//
// Created by Jack D.V. Carson on 9/17/2022.
// Copyright (C) GNU LESSER GENERAL PUBLIC LICENSE
//

#ifndef OPENGCAS_RASTERQUERY_H
#define OPENGCAS_RASTERQUERY_H

#define __DEBUG_VERBOSE
#define BLOCK_SIZE 1024

#include "gdal_priv.h"
#include "structs.h"
#include <vector>
#include <array>
#include <iostream>
#include <algorithm>
#include <string>
#include <memory>
#include <cmath>
#include <experimental/filesystem>
#include <fstream>
#include <cassert>


namespace RQS {
    class rqsDataBlock;

    class RasterQuery;

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

        struct rasterBand {
            GDALRasterBand *band;
            int index;
        };

        /**
         * @brief RasterQuery Private Singleton Constructor
         */
        RasterQuery() = default;

        /**
         * @brief Destructor for RasterQuery. Frees db[9]
         */
        ~RasterQuery();

        /**
         * @brief define m_dataDirTransform vector attribute with
         * geoTransForm data from data/ directory
         */
        auto readDataDir() -> std::vector<geoTransformData>;

        /**
         * @brief returns nPoint origin of each rqsDataBlock in array
         * @see defineCallOrder()
         */
        inline auto
        getBlockLocation(RQS::structures::llPoint loc, int raster, int posX, int posY) -> RQS::structures::nPoint;

        inline auto getClosest(const RQS::structures::llPoint &loc) -> int;

        // Array of rqsDataBlock from which information can be read
        std::array<std::unique_ptr<RQS::rqsDataBlock>, 9> db;

        friend class rqsDataBlock;

    protected:
        // Vector of data/ geoTransformData
        // Primarily used by discreteIndex
        std::vector<geoTransformData> m_dataDirTransform;

        //Vector of open RasterBands based on geospatial position
        std::array<rasterBand, 9> m_rasterCallOrder;

        // Origins of each dataBlock stored in matrix
        std::array<RQS::structures::nPoint, 9> m_dbOrigins;

    public:
        /**
         * @brief Singleton Instance method
         */
        static RasterQuery &get();

        /**
         * @brief Allocate memory and define the call order for rasters on the heap
         * @param loc
         */
        void init(const RQS::structures::llPoint &loc);

        /**@brief Convert llPoint into discrete nPoint on a raster
         *
         * @param llPoint to convert
         * @return nPoint of closest raster index
         */
        auto discreteIndex(const RQS::structures::llPoint &loc) -> RQS::structures::nPoint;

        auto toLL(const RQS::structures::nPoint& nLoc) -> RQS::structures::llPoint;

        /**
         * @brief FOR UNIT TESTING searches through m_dataDirTransform for index of raster by name
         *
         * Highly unoptimized brute force search to ensure unit tests do not need to be refactored.
         * Do not use in production code
         * @param std::string filename
         * @return int index of desired raster passed by filename
         */
        auto searchRasterIndex(const std::string &filename) -> int;

        /**
         * @brief Define a list of Raster files from which data might be realistically found in a
         * 3x3 grid.
         * @param llLocation is llPoint of current location
         */
        auto defineCallOrder(const RQS::structures::llPoint &llLocation) -> std::array<rasterBand, 9>;

        // m_dataDirTransform getter
        auto getDataTransform() -> std::vector<geoTransformData>;

        auto offsetLL(const RQS::structures::llPoint &loc,
                      int offX, int offY,
                      const std::vector<geoTransformData> &dat) -> RQS::structures::llPoint;

        auto getDB(int index) { return db[index].get(); }
    };


//==================================================================


/**
 * @brief Raster Query System Data Block
 *
 * Holds 1024x1024 chunk of data for RQS. Class init allocates memory in the heap
 * for the data
 */
    class rqsDataBlock {
    private:

        /**
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

        /**
         * @brief Reads from raster call stack to fill memory block
         */
        void readFromRaster();

        // Attributes inherited from the singleton reference RasterQuery
        std::vector<RasterQuery::geoTransformData> *m_rqsDataInfo;
        std::array<RasterQuery::rasterBand, 9> *m_rqsCallOrder;

    public:
        RQS::structures::nPoint m_origin;
        const int m_id;

        /**
         * @brief Basic constructor calling init memory functions of rqsDataBlock
         * @param int id for memory alloc
         * @param int posX is x location in 3x3 array
         * @param int posY is y location in 3x3 array
         * @param RasterQuery& rq is reference to RasterQuery singleton
         * @param nPoint origin is top left nPointo f raster
         */
        explicit rqsDataBlock(int id, int posX, int posY, RasterQuery &rq, RQS::structures::nPoint origin);


        /**
         * @brief takes advantage of Pilot Greymap image encoding to quickly save _spBlock to an image for
         * debugging purposes. See etc/scripts/pgmtopng.py to convert images to PNG. Image output is saved in
         * etc/output_vis
         */
        void debugWriteBitmap();

        auto getData() -> std::unique_ptr<int[]> * { return _spBlock.get(); }
    };

}
#endif //OPENGCAS_RASTERQUERY_H
