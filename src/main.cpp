#include <iostream>
#include <vector>
#include "rqs/RasterQuery.h"


int main(int argc, char *argv[]) {
    RasterQuery& rq = RasterQuery::get();
    rq.init(llPoint{42.3, -90.2});
    std::cout << "\n\nRASTER " << rq.discreteIndex(llPoint{42.2, -91.2}).r;
    /*
    nPoint test = rq.discreteIndex(llPoint{41.3, -91.2});

    std::cout << test.x << " " << test.y << " " << test.r;

    //rqsDataBlock test2 = rqsDataBlock(1);*/
    return 0;
}
