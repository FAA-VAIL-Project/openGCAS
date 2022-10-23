#include <iostream>
#include <vector>
#include "rqs/RasterQuery.h"


int main(int argc, char *argv[]) {
    RasterQuery& rq = RasterQuery::get();
    rq.init(llPoint{41.9, -90.3});
    return 0;
}
