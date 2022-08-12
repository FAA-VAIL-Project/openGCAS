cmake_minimum_required(VERSION 2.8)
project(openGCAS)

find_package(GDAL REQUIRED)
include_directories(${GDAL_INCLUDE_DIR})

if(GDAL_FOUND)
  include_directories(${GDAL_INCLUDE_DIR})
else(GDAL_FOUND)
  message(WARNING "GDAL NOT FOUND. Skipping GDAL package build...")

add_executable(openGCAS src/main.cpp src/Raster.cpp)
target_link_libraries(openGCAS PRIVATE GDAL::GDAL)