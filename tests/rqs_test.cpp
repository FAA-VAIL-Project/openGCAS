//
// Created by quothbonney on 9/27/22.
//

#include <catch2/catch.hpp>
#include "../src/rqs/RasterQuery.h"

//TODO Make sure no sign problems (get east and west hemisphere)
//TODO Test boundary conditions across rasters
TEST_CASE("Raster Query System") {
    RasterQuery& rqs = RasterQuery::get();
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

        // Test edge points of gabon raster
        llPoint gEdge{1.00014, 9.99986};
        nPoint gE = rqs.discreteIndex(gEdge);
        CHECK((gE.r == 0));
    }
}