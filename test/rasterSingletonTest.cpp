#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include "../include/Raster.h"

TEST_CASE("Test Empty Input") {
    const char* oklahoma = "../data/n33_w094_1arc_v3.tif";
    Raster& r = Raster::Instance(oklahoma);

    //CHECK(r.getInstanceName() == oklahoma);
}