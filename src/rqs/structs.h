//
// Created by Jack D.V. Carson on 10/30/2022.
// Copyright (C) GNU LESSER GENERAL PUBLIC LICENSE
//

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

        llPoint invert() {
            return llPoint{this->lon, this->lat};
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

    };

/// \brief znPoint
    struct geoPoint {
        int x;
        int y;
        int z;
    };
}