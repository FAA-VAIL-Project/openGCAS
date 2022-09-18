//
// Created by quothbonney on 9/17/22.
//

#include "RasterQuery.h"
#include <string>
#include <experimental/filesystem>

RasterQuery& RasterQuery::get() {
    static RasterQuery rq_instance;
    return rq_instance;
}

std::vector<GDALRasterBand> RasterQuery::readDataDir() {
    GDALAllRegister();
    std::vector<GDALRasterBand> working;
    std::string dataDir = "../../data";
    for (const auto& datafile :
      std::experimental::filesystem::directory_iterator(dataDir)) {
        GDALDataset* e_dataset = (GDALDataset*)GDALOpen(datafile.path(), GA_ReadOnly);

    }
}