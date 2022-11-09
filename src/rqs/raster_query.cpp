//
// Created by Jack D.V. Carson on 9/17/2022.
// Copyright (C) GNU LESSER GENERAL PUBLIC LICENSE
//

#include "rqs.h"

#define EPSILON_FLT 0.001
#define RASTER_SIZE 1.0
#define BLOCK_SIZE 1024

using namespace RQS::structures;
using namespace RQS;

RasterQuery& RasterQuery::get() {
    // Return static instance for singleton
    static RasterQuery rq_instance;
    return rq_instance;
}

RasterQuery::~RasterQuery() {
    for(int i = 0; i < db.size(); ++i) {
        // Close each raster on the CallOrder (guaranteed to have the same size as db)
        GDALClose(std::get<0>(m_rasterCallOrder[i]));
    }
}

inline auto RasterQuery::getBlockLocation(llPoint loc, int raster, int posX, int posY) -> nPoint {
    auto select = m_dataDirTransform[raster];
    // origin = center raster position + (position in matrix * BLOCK_SIZE * resolution of each raster data point)
    double lat = loc.lat + (posY * BLOCK_SIZE * select.lat_res);
    double lon = loc.lon + (posX * BLOCK_SIZE * select.lon_res);
    return discreteIndex(llPoint{lat, lon});
}

void RasterQuery::init(const llPoint& loc) {
    m_dataDirTransform = readDataDir();
    m_rasterCallOrder = defineCallOrder(loc);
    int index = 0;
    // Index across a 3x3 matrix
    for(int i = -1; i < 2; ++i) {
        for(int j = -1; j < 2; ++j) {
            // Get origin of DataBlock
            auto ll = offsetLL(loc, i*BLOCK_SIZE, j*BLOCK_SIZE, m_dataDirTransform);
            nPoint origin = discreteIndex(ll);
            // Allocate memory for it in array
            db[index] = std::make_unique<RQS::rqsDataBlock>(index, j, i, *this, origin, ll);
            // Save origin to protected attribute
            m_dbOrigins[index] = origin;
            index++;
        }
    }

    std::cout << "CLOSEST " <<getClosest(llPoint{42.1, -91.4});
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

    // Sorting lambda sort by latitude then longitude
    auto sortRasterByGT = [](const geoTransformData& rhs, const geoTransformData& lhs) {
        if (lhs.lat_o != rhs.lat_o) {
            return lhs.lat_o > rhs.lat_o;
        } else {
            return lhs.lon_o > rhs.lon_o;
        }
    };

    std::sort(GTVec.begin(), GTVec.end(), sortRasterByGT);

#ifdef __DEBUG_VERBOSE
    std::cout << "RASTER INFO \n";
    for(const auto& j : GTVec) {
        std::cout << j.fname << "  |  Lat: " << j.lat_o << "  |  Lon: " << j.lon_o << "\n";
    }
    std::cout << "\n\n";
#endif

    return GTVec;
}

auto RasterQuery::toLL(const RQS::structures::nPoint &nLoc) -> RQS::structures::llPoint {
    auto ras = &m_dataDirTransform[nLoc.r];
    double lat = ras->lat_o + (ras->lat_res*nLoc.x);
    double lon = ras->lon_o + (ras->lon_res*nLoc.y);
    return llPoint{lat, lon};
}

auto RasterQuery::discreteIndex(const llPoint& loc) -> nPoint {
    int lonMax = getClosest(std::move(loc));

    // CHeck to see if point exists within guess
    // Else finalIndex = -1
    if(lonMax >= 0 && lonMax < m_dataDirTransform.size()) {
        geoTransformData rel = m_dataDirTransform[lonMax];
        double latRasterMax = rel.lat_o + (rel.r_ySize * rel.lat_res); // Maximum latitude contained in each raster
        double lonRasterMax = rel.lon_o + (rel.r_xSize * rel.lon_res); // ~~~~~~~ longitude
        if(
                loc.lon >= rel.lon_o &&
                loc.lon <= lonRasterMax &&
                loc.lat <= rel.lat_o &&
                loc.lat >= latRasterMax
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

auto RasterQuery::defineCallOrder(const llPoint& llLocation) -> std::array<_rb_tup , 9> {
    int index = 0;
    std::array<_rb_tup, 9> working;
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
                const char *name = m_dataDirTransform[n.r].fname.c_str();
                GDALDataset *dataset = (GDALDataset*)GDALOpen(name, GA_ReadOnly);
                GDALRasterBand *band = dataset->GetRasterBand(1);
                working[index] = std::make_tuple(band, n.r);
            } else {
                working[index] = std::make_tuple(nullptr, -1);
            }
            index++;
        }
    }
    return working;
}

auto RasterQuery::searchRasterIndex(const std::string& filename) -> int {
    int i = 0;
    for( const auto& t : m_dataDirTransform ) {
        if(t.fname == filename) { return i; }
        i++;
    }
    // not found
    return -1;
}

auto RasterQuery::getDataTransform() -> std::vector <geoTransformData> {
    if(m_dataDirTransform.empty())
        std::cerr << "WARNING: dataTransform not initialized";
    return m_dataDirTransform;
}

auto RasterQuery::getClosest(const llPoint &loc) -> int {

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
    int size = m_dataDirTransform.size();
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
    const llPoint tmp{m_dataDirTransform[lonMax].lat_o,m_dataDirTransform[lonMax].lon_o};
    //assert((lonMax > 0));
    //assert((&loc > &tmp));
    return lonMax;
}

inline auto RasterQuery::offsetLL(const llPoint& loc,
                                  int offX, int offY,
                                  const std::vector<geoTransformData>& dat) -> llPoint {
    int rasIndex = discreteIndex(loc).r;
    if(rasIndex == -1) {
        // If the raster for the data doesn't exist, it grabs the nearest
        // Raster and takes the res values from it
        rasIndex = getClosest(loc);
    }
    return llPoint {
        loc.lat + dat[rasIndex].lat_res * offX,
        loc.lon + dat[rasIndex].lon_res * offY,
    };
}

RQS::structures::llPoint RasterQuery::geoTransformData::maxLL() const {
    double maxLat = this->lat_o + (this->lat_res * this->r_xSize);
    double maxLon = this->lon_o + (this->lon_res * this->r_ySize);
    return llPoint{maxLat, maxLon};
}

auto RasterQuery::defineLLRes(llPoint loc) -> std::tuple<double, double, int> {
    static std::vector<int> l_callOrder{};
    if(l_callOrder.empty()) {
        for (const auto &in: m_rasterCallOrder)
            if (std::get<1>(in) != -2) l_callOrder.push_back(std::get<1>(in));
        std::sort(l_callOrder.begin(), l_callOrder.end());
    }

    // Get the closest raster and check if it exists in the call order.
    // If not, assign it to the next raster in the call order
    int c = RQS::RasterQuery::get().getClosest(loc);
    auto lambIndex = std::lower_bound(l_callOrder.begin(), l_callOrder.end(), c,
                                      [](int const & lhs,  int target) -> bool
                                { return lhs < target; });

    assert(*lambIndex != -1);
    //assert(c == *lambIndex);
    std::cout << "INDEX FOUND: " << *lambIndex << "\n\n";
    return std::make_tuple(m_dataDirTransform[*lambIndex].lat_res, m_dataDirTransform[*lambIndex].lon_res, *lambIndex);
}

