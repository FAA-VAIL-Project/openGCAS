cmake_minimum_required(VERSION 3.14)

SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${openGCAS_SOURCE_DIR}/bin)

add_executable(tests
        ../tests/main.cpp
        ../tests/rqs_test.cpp
        ../src/rqs/RasterQuery.cpp
        )
target_link_libraries(tests PRIVATE Catch2::Catch2 GDAL::GDAL)

