//
// Created by Jack D.V. Carson on 10/30/2022.
// Copyright (C) GNU LESSER GENERAL PUBLIC LICENSE
//

#ifndef OPENGCAS_RQS_STRUCTURES_H
#define OPENGCAS_RQS_STRUCTURES_H

#include <iostream>

namespace RQS::structures {

/// \brief Geographic point defined by latitude (N) and longitude (E)
    struct llPoint {
        double lat;
        double lon;

        bool operator>(const llPoint &right) const;

        llPoint invert();
    };

/// \brief Index information from within a raster
    struct nPoint {
        int x;
        int y;
        int r;

        [[nodiscard]] bool isNullPoint() const;

    };

/// \brief znPoint
    struct geoPoint {
        int x;
        int y;
        int z;
    };



}

std::ostream& operator<<(std::ostream& os, const RQS::structures::llPoint& ll);
std::ostream& operator<<(std::ostream& os, const RQS::structures::nPoint& n);

#endif // OPENGCAS_RQS_STRUCTURES_H