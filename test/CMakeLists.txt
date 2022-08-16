add_executable(test rasterSingletonTest.cpp ../src/Raster.cpp  ../src/PolySelect.cpp)
target_link_libraries(test Catch2::Catch2 GDAL::GDAL)