//
// Created by quothbonney on 8/22/22.
//

#ifndef OPENGCAS_CIRCSELECT_H
#define OPENGCAS_CIRCSELECT_H

#include "../src/rqs/structs.h"
#include "Raster.h"

class CircSelect {
private:
    Raster* raster;
    int radius;
    nPoint center;
    int** rArray;
    geoPoint* selection;

    int circArea();

public:
    int area;

    geoPoint* getSelection();

    CircSelect(Raster& r, int _radius, nPoint _center) noexcept;
};


#endif //OPENGCAS_CIRCSELECT_H
