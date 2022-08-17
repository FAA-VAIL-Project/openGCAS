#include <catch2/catch.hpp>
#include "../include/Raster.h"

TEST_CASE("Singleton Test") {
    SECTION("Test Instance Uniquness") {
        const char *ok = "../../data/oklahoma.tif";
        const char *ga = "../../data/gabon.tif";

        // Make two copies of the same instance and check that they are the same
        Raster& r = Raster::Instance(ok);
        Raster& m = Raster::Instance(ok);
        CHECK(&r == &m);

        Raster& g = Raster::Instance(ga);
        CHECK(&g == &r);
    }
}