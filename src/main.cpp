#include <iostream>
#include <vector>
#include "rqs/RasterQuery.h"


int main(int argc, char *argv[]) {
    RasterQuery& rq = RasterQuery::get();

    nPoint test = rq.discreteIndex(llPoint{1.00014, 32.9999});
    std::cout << test.x << " " << test.y << " " << test.r << "\n";

    return 0;
}
