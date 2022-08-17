cmake_minimum_required(VERSION 3.14)
project(tests)

add_executable(tests
        main.cpp
        singleton_raster_test.cpp
        ../src/Raster.cpp
        ../src/PolySelect.cpp)
target_link_libraries(tests PRIVATE Catch2::Catch2 GDAL::GDAL)