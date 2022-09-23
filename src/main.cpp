#include <iostream>
#include <vector>
#include "rqs/RasterQuery.h"


int main(int argc, char *argv[]) {
    RasterQuery& rq = RasterQuery::get();

    rq.discreteIndex(llPoint{2.0, 12.83});
    return 0;
}
