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
    int guessLat = -1, guessLon = -1;
    int size = dataDirTransform.size();
    int min = 0;
    int max = size - 1;

    // Basic binary search to get the latitude of the guess
    while(min <= max) {
        int mid = (min + max) / 2;
        if(dataDirTransform[mid].lat_o == workingPoint.lat) {
            guessLat = mid;
        } else if(dataDirTransform[mid].lat_o < workingPoint.lat) {
            min = mid + 1;
        } else {
            max = mid - 1;
        }
    }
    // Only define the index if it is not found to be equal to mid
    int firstLat = -1, lastLat = -1;
    if(guessLat == -1)
        guessLat = max;

    firstLat = max;
    lastLat = max;

    // Iterate from the point to get the minimum and maximum indecies with the same latitude from the sorted
    // dataDirTransform vector
    int i = 1, j = 1;
    while(dataDirTransform[guessLat].lat_o == dataDirTransform[guessLat + i].lat_o) {
        if(lastLat + 1 >= 0 && lastLat + 1 < size) {
            lastLat++;
            i++;
        }
    }
    while(dataDirTransform[guessLat].lat_o == dataDirTransform[guessLat - j].lat_o) {
        if(lastLat - 1 >= 0 && lastLat - 1 < size) {
            firstLat--;
            j++;
        }
    }
    int lonMin = firstLat;
    int lonMax = lastLat;

    while(lonMin <= lonMax) {
        int lonMid = (lonMin + lonMax) / 2;
        if(dataDirTransform[lonMid].lon_o == workingPoint.lon) {
            guessLon = lonMid;
        } else if(dataDirTransform[lonMid].lon_o < workingPoint.lon) {
            lonMin = lonMid + 1;
        } else {
            lonMax = lonMid - 1;
        }
    }

    std::cout << lonMax;

}