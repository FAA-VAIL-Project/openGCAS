//
// Created by Jack D.V. Carson on 9/17/2022.
// Copyright (C) GNU LESSER GENERAL PUBLIC LICENSE
//

#include "RasterQuery.h"

#define EPSILON_FLT 0.001
#define RASTER_SIZE 1.0

RasterQuery& RasterQuery::get() {
    static RasterQuery rq_instance;
    return rq_instance;
}

RasterQuery::~RasterQuery() {
    for(int i = 0; i < db.size(); ++i) {
        delete db[i];
    }
}

void RasterQuery::init(llPoint llLocation) {
    m_dataDirTransform = readDataDir();
    defineCallOrder(llLocation);
    db[0] = new rqsDataBlock(0, -1, -1, *this);
    db[1] = new rqsDataBlock(1, 0, -1, *this);
    db[2] = new rqsDataBlock(2, 1, -1, *this);
    db[3] = new rqsDataBlock(3, -1, 0, *this);
    db[4] = new rqsDataBlock(4, 0, 0, *this);
    db[5] = new rqsDataBlock(5, 1, 0, *this);
    db[6] = new rqsDataBlock(6, -1, 1, *this);
    db[7] = new rqsDataBlock(7, 0, 1, *this);
    db[8] = new rqsDataBlock(8, 1, 1, *this);
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
        std::string extension = s_filename.substr(s_filename.find_last_of('.') + 1);

        // Make sure file extension is correct for gtif data
        if(extension == "tif" || extension == "tiff") {
            auto e_dataset = (GDALDataset *) GDALOpen(filename, GA_ReadOnly);
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
        std::cout << j.fname << "  Lat: " << j.lat_o << " Lon: " << j.lon_o << "\n";
    }
    return GTVec;
}

auto RasterQuery::discreteIndex(llPoint workingPoint) -> nPoint {
    int guessLat = -1;
    int size = m_dataDirTransform.size();
    int min = 0;
    int max = size - 1;

    // Basic binary search to get the latitude of the guess
    while(min <= max) {
        int mid = (min + max) / 2;
        // Test if values are equal by comaring to a float
        if(abs(m_dataDirTransform[mid].lat_o - workingPoint.lat) < EPSILON_FLT) {
            // If point is found to be equal
            guessLat = mid;
            break;
        } else if(m_dataDirTransform[mid].lat_o < workingPoint.lat) {
            min = mid + 1;
        } else {
            max = mid - 1;
        }
    }

    // Define index to be the point after the maximum possible.
    if(guessLat == -1)
        guessLat = max + 1;

    int firstLat = guessLat;
    int lastLat = guessLat;

    // Iterate from the point to get the minimum and maximum indecies with the same latitude from the sorted
    // dataDirTransform vector
    int i = 1, j = 1;
    while(m_dataDirTransform[guessLat].lat_o == m_dataDirTransform[guessLat + i].lat_o) {
        lastLat++;
        i++;
    }
    while(m_dataDirTransform[guessLat].lat_o == m_dataDirTransform[guessLat - j].lat_o) {
        firstLat--;
        j++;
    }
    int lonMin = firstLat;
    int lonMax = lastLat;

    // Now binary search on the range from firstLat to lastLat
    while(lonMin <= lonMax) {
        int lonMid = (lonMin + lonMax) / 2;
        if(abs(m_dataDirTransform[lonMid].lon_o - workingPoint.lon) < EPSILON_FLT) {
            lonMax = lonMid;
            break;
        } else if(m_dataDirTransform[lonMid].lon_o < workingPoint.lon) {
            lonMin = lonMid + 1;
        } else {
            lonMax = lonMid - 1;
        }
    }

    // CHeck to see if point exists within guess
    // Else finalIndex = -1
    if(lonMax >= 0 && lonMax < size) {
        geoTransformData rel = m_dataDirTransform[lonMax];
        double latRasterMax = rel.lat_o + (rel.r_ySize * rel.lat_res);
        double lonRasterMax = rel.lon_o + (rel.r_xSize * rel.lon_res);
        if(
                workingPoint.lat - latRasterMax >= EPSILON_FLT &&
                workingPoint.lat - rel.lat_o <= EPSILON_FLT &&
                workingPoint.lon <= lonRasterMax &&
                abs(workingPoint.lon - rel.lon_o) >= 0
                ) {
            // Actually find the closest point
            int latIndex = (workingPoint.lat - rel.lat_o) / rel.lat_res;
            int lonIndex = (workingPoint.lon - rel.lon_o) / rel.lon_res;
            return nPoint{lonIndex, latIndex, lonMax};
        } else {
            return nPoint{0, 0, -1};
        }
    }
    return nPoint{0, 0, -1};
}

void RasterQuery::defineCallOrder(llPoint llLocation) {
    for(int i = -1; i < 2; i++) {
        for(int j = -1; j < 2; ++j) {
            llPoint p{llLocation.lat - (i * RASTER_SIZE), llLocation.lon + (j * RASTER_SIZE)};
            nPoint n = discreteIndex(p);
            m_rasterCallOrder.push_back(n.r);
        }
    }
}