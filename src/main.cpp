//#define CATCH_CONFIG_MAIN
//#include <catch2/catch.hpp>
#include <eigen3/Eigen/Dense>
#include "../include/Raster.h"
#include <cassert>
#include <iostream>

bool intersections(std::vector<point> p) {

    int counter = 0;
    for(int i = 0; i < p.size(); ++i) {
        for(int j = 0; j < p.size(); ++j) {
            if(i == j)
                continue;

            Eigen::Matrix2f mat;


            const point p1 = p[i];
            const point p2 = p[(i + 1) % p.size()];

            const point p3 = p[j % p.size()];
            const point p4 = p[(j + 1) % p.size()];

            float m1 = (p2.y - p1.y) / (p2.x - p1.x);
            float b1 = p2.y - (m1 * p2.x);
            float b1_alt = p1.y - (m1 * p1.x);
            assert(b1 == b1_alt);

            float m2 = (p4.y - p3.y) / (p4.x - p3.x);
            float b2 = p4.y - (m2 * p4.x);
            float b2_alt = p3.y - (m2 * p3.x);
            assert(b2 == b2_alt);

            Eigen::Vector2f col1{-m1, -m2};
            Eigen::Vector2f col2{1, 1};
            Eigen::Vector2f b{b1, b2};
            mat << col1, col2;
            Eigen::Vector2f x = mat.fullPivLu().solve(b);



        }
    }
    std::cout << counter;
    return 0;
}

int main() {
    std::vector<point> p = {
            {1, 2},
            {4, 5},
            {3, 3},
            {2, -2}
    };
    intersections(p);
}