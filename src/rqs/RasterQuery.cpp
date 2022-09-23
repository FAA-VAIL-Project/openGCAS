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

RasterQuery::RasterQuery() {
    dataDirTransform = readDataDir();
}

auto RasterQuery::readDataDir() -> std::vector<geoTransformData> {
    GDALAllRegister();
    std::vector<GDALRasterBand> working;
    std::string dataDir = "../data";
    int rasterIndex = 0;
    std::vector<geoTransformData> GTVec;

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
            GTVec.push_back(geoTransformData{
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
            GDALClose(e_dataset);
        }
    }
    // Sorting lambda sort by latitude then longitude
    auto sortRasterByGT = [](const geoTransformData& rhs, const geoTransformData& lhs) {
        if (lhs.lat_o != rhs.lat_o) {
            return lhs.lat_o > rhs.lat_o;
        } else {
            return lhs.lon_o > rhs.lon_o;
        }
    };

    std::sort(GTVec.begin(), GTVec.end(), sortRasterByGT);

    for(const auto& j : GTVec) {
        std::cout << "Lat: " << j.lat_o << " Lon: " << j.lon_o << "\n";
    }
    return GTVec;
}

auto RasterQuery::discreteIndex(llPoint workingPoint) -> nPoint {
    int guess_lat = -1, guess_lon = -1;
    int min = 0;
    int max = dataDirTransform.size() - 1;

    // Basic binary search to get the latitude of the guess
    while(min <= max) {
        int mid = (min + max) / 2;
        if(dataDirTransform[mid].lat_o == workingPoint.lat) {
            guess_lat = mid;
        } else if(dataDirTransform[mid].lat_o < workingPoint.lat) {
            min = mid + 1;
        } else {
            max = mid - 1;
        }
    }
    // Only define the index if it is not found to be equal to mid
    if(guess_lat == -1) {
        guess_lat = max + 1;
    }

    min = 0;
    max = dataDirTransform.size() - 1;
    while(min <= max) {
        int mid = (min + max) / 2;
        if(dataDirTransform[mid].lon_o == workingPoint.lon) {
            guess_lon = mid;
        } else if(dataDirTransform[mid].lon_o < workingPoint.lon) {
            min = mid + 1;
        } else {
            max = mid - 1;
        }
    }
    if(guess_lon == -1) {
        guess_lon = max + 1;
    }
    std::cout << guess_lat << " " << guess_lon << "\n";

}