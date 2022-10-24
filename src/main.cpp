#include <iostream>
#include <vector>
#include "rqs/RasterQuery.h"

using namespace RQS::structures;

int main(int argc, char *argv[]) {
    RasterQuery& rq = RasterQuery::get();
    rq.init(llPoint{0.9, -0.3});
    return 0;
}
