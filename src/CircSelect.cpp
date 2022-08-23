//
// Created by quothbonney on 8/22/22.
//

#include "../include/CircSelect.h"
#include <cmath>  // For atan2() and pow()
#include <iostream>


int CircSelect::circArea() {
    // Algebraic simplification of the Gauss Circle Problem
    // Determines the number of squares to cover interior of circle
    // No empty memory
    double pi = 3.14159;
    int area = (pi * radius*radius) - (pi * 2*radius)/(sqrt(2));
    return area;
}

geoPoint* CircSelect::getSelection() {
    selection = new geoPoint[area];

    int index = 0;
    // Iterate across the radius of the circle
    for(int y = 0; y < radius + 1; y++) {
        // x = sqrt(r^2 - y^2)
        // iterate from 0 to root
        for(int i = 0; i < sqrt(radius*radius - y*y) + 1; ++i) {
            point pos{center.x + i, center.y + y};
            point neg{center.x - i, center.y - y};

            // create negative and positive point, create a geoPoint referencing raster
            // Else, altitude = 0
            if(pos.x >= 0 && pos.x < raster->xSize && pos.y >=0 && pos.y < raster->ySize) {
                selection[index] = geoPoint{pos.x, pos.y, rArray[pos.y][pos.x]};
            } else {
                selection[index] = geoPoint{pos.x, pos.y, 0};
            }
            index++;
            if(neg.x >= 0 && neg.x < raster->xSize && neg.y >=0 && neg.y < raster->ySize) {
                selection[index] = geoPoint{neg.x, neg.y, rArray[neg.y][neg.x]};
            } else {
                selection[index] = geoPoint{neg.x, neg.y, 0};
            }
            index++;
        }
    }
    return selection;
}

CircSelect::CircSelect(Raster& r, int _radius, point _center) noexcept
: radius(_radius), center(_center), raster(&r), rArray(r.getArray()) {

    // Get area
    this->area = circArea();

}
