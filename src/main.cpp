#include <iostream>
#include <vector>
#include "rqs/RasterQuery.h"


int main(int argc, char *argv[]) {
    RasterQuery& rq = RasterQuery::get();

    rq.discreteIndex(llPoint{35, -90});
    return 0;
}
