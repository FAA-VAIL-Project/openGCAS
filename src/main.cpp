#include <iostream>
#include <vector>
#include "rqs/RasterQuery.h"


int main(int argc, char *argv[]) {
    RasterQuery& rq = RasterQuery::get();

    rq.discreteIndex(llPoint{20, 10});
    return 0;
}
