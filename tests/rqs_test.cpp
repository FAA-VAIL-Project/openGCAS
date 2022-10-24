//
// Created by quothbonney on 9/27/22.
//

#include <catch2/catch.hpp>
#include "../src/rqs/RasterQuery.h"

using namespace RQS::structures;

TEST_CASE("RasterQuery Unit Tests") {
    RasterQuery& rqs = RasterQuery::get();
    rqs.init(llPoint{41.9, -90.3});
    SECTION("Read Saved Raster Data") {
        // Check that adding a non raster wont break reading
        std::ofstream testNonRaster;
        const char* fp = "../data/not_a_raster.jpg";
        testNonRaster.open(fp);
        rqs.init(llPoint{41.9, -90.3});
        auto dt = rqs.getDataTransform();
        CHECK(dt.size() == 9);

        // Check sorted
        for(int i = 1; i < dt.size(); ++i) {
            bool gr = (llPoint{dt[i].lat_o, dt[i].lon_o} > llPoint{dt[i-1].lat_o, dt[i-1].lon_o});
            CHECK(gr);
        }
        std::experimental::filesystem::remove(fp);
    }
    SECTION("Discrete Index") {
        // Create impossible points and test to guaruntee that they are do not exist
        llPoint impossible1{900.283, -1923.2};
        llPoint impossible2{100.0, -328.f};
        nPoint i1r = rqs.discreteIndex(impossible1);
        nPoint i2r = rqs.discreteIndex(impossible2);
        CHECK(i1r.isNullPoint());
        CHECK(i2r.isNullPoint());

        // Create points inside of gabon.tif assuming
        int indGabon = rqs.searchRasterIndex("../data/unit1.tif");
        llPoint gabon1{0.5, 10.5};
        llPoint gabon2{0.9, 10.1};
        nPoint g1r = rqs.discreteIndex(gabon1);
        nPoint g2r = rqs.discreteIndex(gabon2);
        // Check that points are within the Gabon raster
        CHECK(g1r.r == indGabon);
        CHECK(g2r.r == indGabon);

        int t;
        for(int i = 0; i < 1000; ++i) {
            llPoint test{41.5, -90.5 + (i/501)};
            if(rqs.discreteIndex(test).r == -1) t = -1;
        }
        CHECK(t != -1);
    }
    SECTION("Raster Call Order") {
        for(double i = -10; i < 10; i+=0.1) {
            llPoint t = llPoint{i, -i};
            // Check that the origin exists (always should)
            auto arr = rqs.defineCallOrder(t);
            CHECK((arr[4].index == rqs.discreteIndex(t).r));
            std::vector<int> existingIndecies;

            // Check no duplicate rasters
            for(int j = 0; j < 9; ++j) {
                if(arr[j].index > -1) {
                    bool exists = std::find(
                            existingIndecies.begin(), existingIndecies.end(), arr[j].index
                            ) != existingIndecies.end();
                    CHECK(exists == false);
                    existingIndecies.push_back(arr[j].index);
                }
            }
        }
    }
}