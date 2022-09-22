//
// Created by quothbonney on 9/17/22.
//

#include "RasterQuery.h"
#include <string>
#include <experimental/filesystem>
#include "gdal_priv.h"
#include <algorithm>
#include <iostream>

RasterQuery& RasterQuery::get() {
    static RasterQuery rq_instance;
    return rq_instance;
}

void RasterQuery::readDataDir() {
    GDALAllRegister();
    std::vector<GDALRasterBand> working;
    std::string dataDir = "../data";
    int rasterIndex = 0;
    for (const auto& datafile :
      std::experimental::filesystem::directory_iterator(dataDir)) {
        // Iterate across all files in the data directory
        std::string pathstring{datafile.path().string()};
        const char* filename = datafile.path().c_str();
        std::string s_filename = datafile.path().string();
        // Neat string parsing thing to get file extension
        std::string extension = s_filename.substr(s_filename.find_last_of(".") + 1);

        // Make sure file extension is correct for gtif data
        if(extension == "tif" || extension == "tiff") {
            GDALDataset* e_dataset = (GDALDataset *) GDALOpen(filename, GA_ReadOnly);
            double GDALTransfom[6];
            e_dataset->GetGeoTransform(GDALTransfom);
            // Push the relevant GeoTransform data to the protected attribute dataDirTransform
            dataDirTransform.push_back(geoTransformData{
                rasterIndex,
                filename,
                GDALTransfom[0],
                GDALTransfom[1],
                GDALTransfom[3],
                GDALTransfom[5],
                e_dataset->GetRasterXSize(),
                e_dataset->GetRasterYSize(),
            });

            rasterIndex++;
        }
    }
}