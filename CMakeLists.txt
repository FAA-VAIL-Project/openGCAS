cmake_minimum_required(VERSION 3.14)
project(openGCAS)

find_package(GDAL REQUIRED)
include(FetchContent)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")

if(GDAL_FOUND)
    include_directories(${GDAL_INCLUDE_DIR})
else(GDAL_FOUND)
    message(WARNING "GDAL NOT FOUND. Skipping GDAL package build...")
endif()

link_libraries(stdc++fs)

#add_subdirectory(dependencies)
#add_subdirectory(tests)

FetchContent_Declare(SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

add_executable(openGCAS
        src/main.cpp
        src/rqs/raster_query.cpp
        src/rqs/rqs_datablock.cpp
        src/rqs/sfml_front.cpp src/rqs/sfml_front.h src/rqs/structs.cpp)
target_link_libraries(openGCAS PRIVATE GDAL::GDAL)
target_link_libraries(openGCAS PRIVATE sfml-graphics)

target_compile_features(openGCAS PRIVATE cxx_std_17)
target_precompile_headers(openGCAS
        PUBLIC
        src/rqs/rqs.h
        )

FetchContent_Declare(SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)