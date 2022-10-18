//
// Created by Jack D.V. Carson on 9/17/2022.
// Copyright (C) GNU LESSER GENERAL PUBLIC LICENSE
//

#include "RasterQuery.h"

#define EPSILON_FLT 0.001
#define RASTER_SIZE 1.0
#define BLOCK_SIZE 1024

RasterQuery& RasterQuery::get() {
    // Return static instance for singleton
    static RasterQuery rq_instance;
    return rq_instance;
}

RasterQuery::~RasterQuery() {
    for(int i = 0; i < db.size(); ++i) {
        // Delete each rqsDataBlock object
        delete db[i];
        // Close each raster on the CallOrder (guaranteed to have the same size as db)
        GDALClose(m_rasterCallOrder[i].band);
    }
}

inline auto RasterQuery::getBlockLocation(llPoint location, int raster, int posX, int posY) -> nPoint {
    auto select = m_unsortedDirTransform[raster];
    // origin = center raster position + (position in matrix * BLOCK_SIZE * resolution of each raster data point)
    double lat = location.lat + (posY * BLOCK_SIZE * select.lat_res);
    double lon = location.lon + (posX * BLOCK_SIZE * select.lon_res);
    return discreteIndex(llPoint{lat, lon});
}

void RasterQuery::init(const llPoint& llLocation) {
    m_dataDirTransform = readDataDir();
    defineCallOrder(llLocation);
    int locRaster = discreteIndex(llLocation).r;
    int index = 0;
    // Index across a 3x3 matrix
    for(int i = -1; i < 2; ++i) {
        for(int j = -1; j < 2; ++j) {
            // Get origin of DataBlock
            nPoint origin = getBlockLocation(llLocation, locRaster, j, i);
            // Allocate memory for it in array
            db[index] = new rqsDataBlock(index, j, i, *this, origin);
            // Save origin to protected attribute
            m_dbOrigins[index] = origin;
            index++;
        }
    }
}

auto RasterQuery::readDataDir() -> std::vector<geoTransformData> {
    // Init GDAL and local variables
    GDALAllRegister();
    std::string dataDir = "../data";
    int rasterIndex = 0;
    std::vector<geoTransformData> GTVec;

    // Iterate over filenames in the data/ root directory
    for (const auto& datafile :
      std::experimental::filesystem::directory_iterator(dataDir)) {
        std::string pathstring{datafile.path().string()};
        // Save both the c_str() and the string()
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
                s_filename,
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

    // Save the unsorted vector and continue working on GTVec
    m_unsortedDirTransform = GTVec;

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

auto RasterQuery::discreteIndex(const llPoint& loc) -> nPoint {
    /*
     * Basically what is happening here: a wacky 2d binary search. Latitude is the first aspect checked,
     * but because binary search doesn't consider the fact that there may be multiple instances of the same
     * latitude, it chooses whichever index it lands on first and calls it the guess. Therefore, two
     * loops iterate up and down the m_dataDirTransform vector to find the first and last indicies with the same latitude.
     * After that, a second binary search is done ONLY between those two indicies, and ONLY considering the
     * longitude of each raster. Over large amounts of rasters (as would be seen in a real use-case, this is
     * faster than a brute force search as it scales logarithmically).
     */
    int guessLat = -1;
    int size = m_unsortedDirTransform.size();
    int min = 0;
    int max = size - 1;

    // Basic binary search to get the latitude guess
    while(min <= max) {
        int mid = (min + max) / 2;
        // Test if values are equal by comaring to a float
        if(abs(m_dataDirTransform[mid].lat_o - loc.lat) < EPSILON_FLT) {
            // If point is found to be equal
            guessLat = mid;
            break;
        } else if(m_dataDirTransform[mid].lat_o < loc.lat) {
            min = mid + 1;
        } else {
            max = mid - 1;
        }
    }

    // Assuming that the pointw as not found exactly, define it as the max + 1
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
        if(abs(m_dataDirTransform[lonMid].lon_o - loc.lon) < EPSILON_FLT) {
            lonMax = lonMid;
            break;
        } else if(m_dataDirTransform[lonMid].lon_o < loc.lon) {
            lonMin = lonMid + 1;
        } else {
            lonMax = lonMid - 1;
        }
    }

    // CHeck to see if point exists within guess
    // Else finalIndex = -1
    if(lonMax >= 0 && lonMax < size) {
        geoTransformData rel = m_dataDirTransform[lonMax];
        double latRasterMax = rel.lat_o - (rel.r_ySize * rel.lat_res); // Maximum latitude contained in each raster
        double lonRasterMax = rel.lon_o + (rel.r_xSize * rel.lon_res); // ~~~~~~~ longitude
        if(
                loc.lat - latRasterMax <= EPSILON_FLT && // If latitude - maxLat is negative (lat increments negatively)
                loc.lat - rel.lat_o <= EPSILON_FLT && // If latitude exists inside raster
                loc.lon <= lonRasterMax && // If longitude is less than max longitude
                abs(loc.lon - rel.lon_o) >= 0 // If longitude exists inside raster
                ) {
            // Actually find the closest point
            int latIndex = (loc.lat - rel.lat_o) / rel.lat_res;
            int lonIndex = (loc.lon - rel.lon_o) / rel.lon_res;
            return nPoint{lonIndex, latIndex, lonMax};
        } else {
            // Return null point if point is not within a raster
            return nPoint{0, 0, -1};
        }
    }
    // Return null if something has catastrophically failed and the above if statement is false
    return nPoint{0, 0, -1};
}

void RasterQuery::defineCallOrder(llPoint llLocation) {
    int index = 0;
    for(int i = -1; i < 2; i++) {
        for(int j = -1; j < 2; ++j) {
            /*
             * Kinda crude method of assuming each raster is RASTER_SIZE (1.0 lat-lon for current implementation)
             * and getting each surrounding raster by adding RASTER_SIZE to the location llPoint and discreteIndex
             * the new llPoint to get the raster
             */
            llPoint p{llLocation.lat - (i * RASTER_SIZE), llLocation.lon + (j * RASTER_SIZE)};
            nPoint n = discreteIndex(p);



            if(!n.isNullPoint()) {
                const char *name = m_unsortedDirTransform[n.r].fname.c_str();
                GDALDataset *dataset = (GDALDataset*)GDALOpen(name, GA_ReadOnly);
                GDALRasterBand *band = dataset->GetRasterBand(1);
                m_rasterCallOrder[index] = rasterBand{band, n.r};
            } else {
                m_rasterCallOrder[index] = rasterBand{nullptr, -1};
            }
            index++;
        }
    }
}