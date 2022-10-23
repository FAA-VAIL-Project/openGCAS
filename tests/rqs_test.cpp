//
// Created by quothbonney on 9/27/22.
//

#include <catch2/catch.hpp>
#include "../src/rqs/RasterQuery.h"

TEST_CASE("RasterQuery Unit Tests") {
    RasterQuery& rqs = RasterQuery::get();
    rqs.init(llPoint{0.0, 0.0});
    SECTION("Discrete Index") {
        // Create impossible points and test to guaruntee that they are do not exist
        llPoint impossible1{900.283, -1923.2};
        llPoint impossible2{1000, -328};
        nPoint i1r = rqs.discreteIndex(impossible1);
        nPoint i2r = rqs.discreteIndex(impossible2);
        CHECK(i1r.isNullPoint());
        CHECK(i2r.isNullPoint());

        // Create points inside of gabon.tif assuming
        llPoint gabon1{0.5, 10.5};
        llPoint gabon2{0.9, 10.1};
        nPoint g1r = rqs.discreteIndex(gabon1);
        nPoint g2r = rqs.discreteIndex(gabon2);
        // Check that points are within the Gabon raster
        CHECK(g1r.r == 0);
        CHECK(g2r.r == 0);

        int t;
        for(int i = 0; i < 1000; ++i) {
            llPoint test{41.5, -90.5 + (i/501)};
            if(rqs.discreteIndex(test).r == -1) t = -1;
        }
        CHECK(t != -1);
    }
}