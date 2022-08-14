//
// Created by quothbonney on 8/13/22.
//

#ifndef OPENGCAS_POLYSELECT_H
#define OPENGCAS_POLYSELECT_H

#include "include/structs.h"
#include "include/Raster.h"

class PolySelect {
private:
    Raster* raster;
    std::vector<point> points;
    int** rArray;


    // Operator predicate passed to std::sort
    // Calculates the theta in the polar coordinate of point& input
    // and sorts based on theta without creating a new vector in memory
    struct sort {
        point center;
        bool operator()(point const& lhs, point const& rhs) {
            float lhsTheta = atan2(lhs.y - center.y, lhs.x - center.x);
            float rhsTheta = atan2(rhs.y - center.y, rhs.x - center.x);
            // If theta is the same (colinear) pick the larger radius
            if(lhsTheta == rhsTheta) {
                float lhsR = pow(lhs.x - center.x, 2) + pow(lhs.y - center.y, 2); // Square of radius
                float rhsR = pow(rhs.x - center.x, 2) + pow(rhs.y - center.y, 2);
                return lhsR < rhsR;
            }
            return lhsTheta < rhsTheta;
        }
    };


    // Ensures that the points are within the array (prevents memory violations)
    // TODo: Allow for invalid indexes and just create 0's
    void isIndexable() {
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

    // Define center point for polar coords
    point defineCenter() {
        int workingX = 0, workingY = 0;

        for( const auto &p : points) {
            workingX += p.x;
            workingY += p.y;
        }
        point c{workingX / 4, workingY / 4};
        return c;
    }


    bool withinPoly(point p) {
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

    int polyArea() {
        area = 0;

        for(int i = 0, j = points.size() - 1; i < points.size(); i++) {
            area += (points[j].x + points[i].x) * (points[j].y - points[i].y);
            j = i;
        }

        area = abs(area / 2);
        return area;
    }

public:
    int area;

    geoPoint* getSelection() {
        geoPoint* selection = new geoPoint[area];
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

    PolySelect(Raster &r, std::vector<point> pointVec) {
        this->points = pointVec;
        this->raster = &r;

        this->rArray = r.getArray();

        isIndexable();

        // Init object referenced in std::sort
        // Sort OVER points (no copy)
        sort polarVectorSort{defineCenter()};
        std::sort(points.begin(), points.end(), polarVectorSort);

        this->area = polyArea();
        geoPoint* selection = new geoPoint[area];
        int index = 0;
        for(int row = 0; row < r.ySize; row++) {
            for(int elem = 0; elem < r.xSize; elem++) {
                point temp{elem, row};
                if (!withinPoly(temp)) {
                    selection[index] = geoPoint{elem, row, rArray[row][elem]};
                    index++;
                }
            }
        }
    }
};


#endif //OPENGCAS_POLYSELECT_H
