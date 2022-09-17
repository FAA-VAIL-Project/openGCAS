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

    // Manually set the first position
    int index = 1;

    // Iterate across the radius of the circle
    for(int y = 0; y < radius; y++) {
        // x = sqrt(r^2 - y^2)
        // iterate from 0 to root
        float root = std::round(sqrt(radius*radius - y*y));
        for(int i = -1 * root; i < root; ++i) {
            // Define the location of the points to each side of the centerF
            point p{center.x + i, center.y + y};
            point p2{center.x + i, center.y - y};

            // create negative and positive point, create a geoPoint referencing raster
            // Else, altitude = 0
            if(p.x >= 0 && p.x < raster->xSize && p.y >=0 && p.y < raster->ySize) {
                selection[index] = geoPoint{p.x, p.y, rArray[p.y][p.x]};
                index++;
            } else {
                selection[index] = geoPoint{p.x, p.y, 0};
                index++;
            }
            if(!(p.x == p2.x && p.y == p2.y)) {
                if (p2.x >= 0 && p2.x < raster->xSize && p2.y >= 0 && p2.y < raster->ySize) {
                    selection[index] = geoPoint{p2.x, p2.y, rArray[p2.y][p2.x]};
                    index++;
                } else {
                    selection[index] = geoPoint{p2.x, p2.y, 0};
                    index++;
                }
            }
        }
        }
    return selection;
}

CircSelect::CircSelect(Raster& r, int _radius, point _center) noexcept
: radius(_radius), center(_center), raster(&r), rArray(r.getArray()) {

    // Get area
    this->area = 100;

    getSelection();

    for(int i = 0; i < area; ++i) {
        std::cout << selection[i].x << " " << selection[i].y << " " << selection[i].z << "\n";
    }

}
