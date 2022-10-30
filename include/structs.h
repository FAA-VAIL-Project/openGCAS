//
// Define global structs
//

#ifndef OPENGCAS_STRUCTS_H
#define OPENGCAS_STRUCTS_H

#include <iostream>

namespace RQS::structures {

/// \brief Geographic point defined by latitude (N) and longitude (E)
    struct llPoint {
        double lat;
        double lon;

        bool operator>(const llPoint &right) {
            bool eq = abs(this->lat - right.lat) <= 0.001;
            if (this->lat > right.lat || eq) {
                if (eq) {
                    return this->lon < right.lon;
                }
                return true;
            }
            return false;
        };
    };

/// \brief Index information from within a raster
    struct nPoint {
        int x;
        int y;
        int r;

        bool isNullPoint() {
            return (r == -1);
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
}
#endif //OPENGCAS_STRUCTS_H
