//
// Define global structs
//

#ifndef OPENGCAS_STRUCTS_H
#define OPENGCAS_STRUCTS_H

#include <iostream>

/// \brief Geographic point defined by latitude (N) and longitude (E)
struct llPoint {
    double lat;
    double lon;
};

/// \brief Index information from within a raster
struct nPoint {
    int x;
    int y;
    int r;

    bool isNullPoint() {
        return (x == 0 && y == 0 && r == -1);
    };

    //friend std::ostream& operator<<(std::ostream& os, const nPoint& p);
};

// Basic std::cout operator overload for quick print debugging



/// \brief znPoint
struct geoPoint {
    int x;
    int y;
    int z;
};

#endif //OPENGCAS_STRUCTS_H
