//
// Created by quothbonney on 8/13/22.
//

#ifndef OPENGCAS_POLYSELECT_H
#define OPENGCAS_POLYSELECT_H

#include "../src/rqs/structs.h"
#include "Raster.h"

class PolySelect {
private:
    Raster* raster;
    std::vector<nPoint> points;
    int** rArray;
    geoPoint* selection;


    // Operator predicate passed to std::sort
    // Calculates the theta in the polar coordinate of point& input
    // and sorts based on theta without creating a new vector in memory
    struct sort;

    // Define center point for polar coords
    nPoint defineCenter();

    // Test that point is within ordered polygon
    // Returns INVERSE of point p within polygon
    // defined in class attribute points
    bool withinPoly(nPoint p);

    // Gets area of polygon defined with class
    // IMPORTANT: area is used to allocate minimum possible memory for
    // selection.
    int polyArea();

public:
    // Public attribute to allow for easy use in loops
    int area;

    // Returns selection
    // Do not forget to delete[] the geoPoint*
    geoPoint* getSelection();

    // Constructor takes a Raster object and a vector of points
    PolySelect(Raster& r, std::vector<nPoint> pointVec) noexcept;

    // Free selection pointer
    ~PolySelect();
};


#endif //OPENGCAS_POLYSELECT_H
