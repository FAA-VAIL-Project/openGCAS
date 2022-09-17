//
// Created by quothbonney on 9/8/22.
//

#include "../include/RasterQuery.h"

/// \brief Geographic point defined by latitude (N) and longitude (E)
struct RasterQuery::llpoint {
    int lat;
    int lon;
};