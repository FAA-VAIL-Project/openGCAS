cmake_minimum_required(VERSION 3.14)
project(tests)

find_package(Eigen3 REQUIRED)

link_directories(${Eigen_INCLUDE_DIRS})

add_executable(tests
        main.cpp
        ../src/Raster.cpp
        ../src/PolySelect.cpp)
target_link_libraries(tests PRIVATE Catch2::Catch2 GDAL::GDAL ${Eigen_LIBRARIES})

