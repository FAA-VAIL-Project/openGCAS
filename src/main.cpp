#include <iostream>
#include <vector>
#include "rqs/RasterQuery.h"


int main(int argc, char *argv[]) {
    RasterQuery& rq = RasterQuery::get();
    nPoint test = rq.discreteIndex(llPoint{100.3, -91.2});

    std::cout << test.x << " " << test.y << " " << test.r;

    //rqsDataBlock test2 = rqsDataBlock(1);
    return 0;
}
