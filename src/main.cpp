#include <iostream>
#include <vector>
#include "rqs/RasterQuery.h"


int main(int argc, char *argv[]) {
    RasterQuery& rq = RasterQuery::get();

    nPoint test = rq.discreteIndex(llPoint{0.9992, 10.22});
    std::cout << test.x << " " << test.y << " " << test.r << "\n";

    return 0;
}
