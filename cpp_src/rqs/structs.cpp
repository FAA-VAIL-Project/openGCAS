//
// Created by Jack D.V. Carson on 11/1/2022.
// Copyright (C) GNU LESSER GENERAL PUBLIC LICENSE
//

#include "structs.h"

using namespace RQS::structures;

bool llPoint::operator>(const llPoint &right) const {
    bool eq = abs(this->lat - right.lat) <= 0.001;
    if (this->lat > right.lat || eq) {
        if (eq) {
            return this->lon < right.lon;
        }
        return true;
    }
    return false;
};

llPoint llPoint::invert() {
    return llPoint{this->lon, this->lat};
}

bool nPoint::isNullPoint() const {
    return (r == -1);
}

std::ostream& operator<<(std::ostream& os, const nPoint& n) {
    return os << "NPOINT: " << "{" << n.x << ", " << n.y << ", " << n.r << "}\n";
}

std::ostream& operator<<(std::ostream& os, const llPoint& ll) {
    return os << "LLPOINT: " << "{lat: " << ll.lat << ", lon: " << ll.lon << "}\n";
}
