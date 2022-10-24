cmake_minimum_required(VERSION 3.14)
project(openGCAS)

find_package(GDAL REQUIRED)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")

if(GDAL_FOUND)
    include_directories(${GDAL_INCLUDE_DIR})
else(GDAL_FOUND)
    message(WARNING "GDAL NOT FOUND. Skipping GDAL package build...")
endif()

link_libraries(stdc++fs)

add_subdirectory(dependencies)
add_subdirectory(tests)



add_executable(openGCAS
        src/main.cpp
        src/rqs/RasterQuery.cpp
        src/rqs/rqsDataBlock.cpp
        )
target_link_libraries(openGCAS PRIVATE GDAL::GDAL)

target_precompile_headers(openGCAS
        PUBLIC
        src/rqs/RasterQuery.h
        )