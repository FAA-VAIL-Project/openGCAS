#include "../include/PolySelect.h"
#include <vector>
#include <algorithm> // for std::sort()
#include <cmath>  // For atan2() and pow()
#include <iostream>

struct PolySelect::sort {
    nPoint center;
    bool operator()(nPoint const& lhs, nPoint const& rhs) {
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


nPoint PolySelect::defineCenter() {
    // Pretty barebones: just average of x's and y's
    int workingX = 0, workingY = 0;
    for( const auto &p : points) {
        workingX += p.x;
        workingY += p.y;
    }
    nPoint c{workingX / 4, workingY / 4};
    return c;
}


inline bool PolySelect::withinPoly(nPoint p) {
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

    // Creates iterators passed to std::minimax
    std::vector<nPoint>::iterator minX, maxX, minY, maxY;
    std::tie(minX, maxX) = std::minmax_element(
            begin(points), end(points),
            // Comparison predicate as lambda
            [](nPoint const& lhs, nPoint const& rhs)
            {
                return lhs.x < rhs.x;
            }
    );

    std::tie(minY, maxY) = std::minmax_element(
            begin(points), end(points),
            // Comparison predicate as lambda
            [](nPoint const& lhs, nPoint const& rhs)
            {
                return lhs.y < rhs.y;
            }
    );

    // Iterate only from minY to maxY and minX to maxX
    int index = 0;
    for(int row = minY->y; row < maxY->y; row++) {
        for (int elem = minX->x; elem < maxX->x; elem++) {
            nPoint temp{elem, row};
            if(elem > raster->xSize || elem < 0 ||
                row > raster->ySize || row < 0) {
                selection[index] = geoPoint{elem, row, 0};
            }
            else if (!withinPoly(temp)) {
                selection[index] = geoPoint{elem, row, rArray[row][elem]};
                index++;
            }
        }
    }
    return selection;
}

PolySelect::PolySelect(Raster& r, std::vector<nPoint> pointVec) noexcept
    : points(pointVec), raster(&r), rArray(r.getArray()) {
/*
    // Init object referenced in std::sort
    // Sort OVER points (no copy)
    sort polarVectorSort{defineCenter()};
    std::sort(points.begin(), points.end(), polarVectorSort);

    this->area = polyArea();

    getSelection();

    for(int i = 0; i < area; ++i) {
        std::cout << selection[i].z << "\n";
    }
    */
}
PolySelect::~PolySelect() {
    //delete[] selection;
}
