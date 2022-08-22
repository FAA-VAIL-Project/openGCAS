//
// Created by quothbonney on 8/22/22.
//

#ifndef OPENGCAS_CIRCSELECT_H
#define OPENGCAS_CIRCSELECT_H

#include "structs.h"
#include "Raster.h"

class CircSelect {
private:
    Raster* raster;
    int radius;
    point center;
    int** rArray;
    geoPoint* selection;

    int circArea();

public:
    int area;

    geoPoint* getSelection();

    CircSelect(Raster& r, int _radius, point _center) noexcept;
};


#endif //OPENGCAS_CIRCSELECT_H
