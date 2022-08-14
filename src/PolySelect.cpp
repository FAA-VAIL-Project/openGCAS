//
// Created by quothbonney on 8/13/22.
//

#include "../include/PolySelect.h"
#include "../include/structs.h"
#include "../include/Raster.h"
#include <vector>
#include <algorithm> // for std::sort()
#include <iostream>
#include <cmath>  // For atan2() and pow()


struct PolySelect::sort {
    point center;
    bool operator()(point const& lhs, point const& rhs) {
        // Compare the polar coordinate theta of the struct
        float lhsTheta = std::atan2(lhs.y - center.y, lhs.x - center.x);
        float rhsTheta = std::atan2(rhs.y - center.y, rhs.x - center.x);
        // If theta is the same (colinear) pick the larger radius
        if(lhsTheta == rhsTheta) {
            float lhsR = std::pow(lhs.x - center.x, 2) + std::pow(lhs.y - center.y, 2); // Square of radius
            float rhsR = std::pow(rhs.x - center.x, 2) + std::pow(rhs.y - center.y, 2);
            return lhsR < rhsR;
        }
        return lhsTheta < rhsTheta;
    }
};


// TODO: Allow for invalid indexes and just create 0's
void PolySelect::isIndexable() {
    for (const auto &p: points) {
        try {
            if (p.x > raster->xSize || p.x < 0 || p.y > raster->ySize || p.y < 0) {
                throw 100;
            }
        }
        catch (int e) {
            std::cerr << "Cannot create PolySelect! Point " << p.x << " " << p.y << " is outside raster range "
                      << raster->xSize << " " << raster->ySize << "...";
            exit(1);
        }
    }
}


point PolySelect::defineCenter() {
    // Pretty barebones: just average of x's and y's
    int workingX = 0, workingY = 0;
    for( const auto &p : points) {
        workingX += p.x;
        workingY += p.y;
    }
    point c{workingX / 4, workingY / 4};
    return c;
}


bool PolySelect::withinPoly(point p) {
    // Brilliant algorithm modified from https://wrfranklin.org/Research/Short_Notes/pnpoly.html
    bool c = true;
    for (int i = 0, j = points.size() - 1; i < points.size(); j = i++) {
        if (
                ((points[i].y > p.y) != (points[j].y > p.y)) &&
                (p.x < (points[j].x - points[i].x) * (p.y - points[i].y) / (points[j].y - points[i].y) + points[i].x)
                ) c = !c;
    }
    return c;
}

int PolySelect::polyArea() {
    area = 0;
    // Implementation of shoelace formula for polygon size
    // See: https://en.wikipedia.org/wiki/Shoelace_formula
    for(int i = 0, j = points.size() - 1; i < points.size(); i++) {
        area += (points[j].x + points[i].x) * (points[j].y - points[i].y);
        j = i;
    }

    area = abs(area / 2);
    return area;
}


geoPoint* PolySelect::getSelection() {
    selection = new geoPoint[area];
    int index = 0;
    for(int row = 0; row < raster->ySize; row++) {
        for (int elem = 0; elem < raster->xSize; elem++) {
            point temp{elem, row};
            if (!withinPoly(temp)) {
                selection[index] = geoPoint{elem, row, rArray[row][elem]};
                index++;
            }
        }
    }
    return selection;
}

PolySelect::PolySelect(Raster &r, std::vector<point> pointVec) {
    this->points = pointVec;
    this->raster = &r;

    // Just copies array at 0x7fffe95a7010 (Can't reduce memory usage)
    this->rArray = r.getArray();

    isIndexable();

    // Init object referenced in std::sort
    // Sort OVER points (no copy)
    sort polarVectorSort{defineCenter()};
    std::sort(points.begin(), points.end(), polarVectorSort);

    this->area = polyArea();

    // Fills selection pointer with memory
    getSelection();
}

PolySelect::~PolySelect() {
    //delete[] selection;
}
