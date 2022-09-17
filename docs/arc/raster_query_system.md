# Raster Query System

The Raster Query System is a class containing interfaces pertaining the conversion of `llPoint` structs 
(defined in `structs.h`) to indicies on a local raster. It accepts a reference to an `std::vector<GDALRasterBand>`
named `rasterVec` containing the RasterBands for each file in `data/` as well as an `std::vector<llPoint>` named
`llVec`. 

`checkPoint()` ensures that each llPoint index in llVec is a possible value. If an impossible value is detected, it will
pop it from the vector. This method is called in the initializer list of the RQS class to prevent any issues when copying the
vector.

`discreteIndex()` creates a new `std::array<int, 3>` vector named `nPointInd` with the size of llVec that contains
the index in rasterVec which the same-indexed point in llVec lives as the third index, and the i and j index on that raster
as the first and second index.

The first goal, before worrying about selections and selection interpolation density, is to ensure and test the indexability
of each and every possible lat-long point.