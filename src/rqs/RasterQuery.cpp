//
// Created by quothbonney on 9/17/22.
//

#include "RasterQuery.h"
#include <string>
#include <experimental/filesystem>
#include "gdal_priv.h"
#include <algorithm>

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
        std::string pathstring{datafile.path().string()};
        const char* filename = datafile.path().c_str();
        std::string s_filename = datafile.path().string();
        std::string extension(std::find(s_filename.begin(), s_filename.end(), '.'), s_filename.end());
        if(extension == "tif" && extension == "tiff") {
            GDALDataset* e_dataset = (GDALDataset *) GDALOpen(filename, GA_ReadOnly);
        }
    }
}