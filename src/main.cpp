#include <iostream>
#include <vector>
#include "rqs/RasterQuery.h"


int main(int argc, char *argv[]) {
    RasterQuery& rq = RasterQuery::get();
/*
    nPoint test = rq.discreteIndex(llPoint{0.5,10.2});
    std::cout << test.x << " " << test.y << " " << test.r << "\n";
*/
    rqsDataBlock test = rqsDataBlock(1);
    return 0;
}
