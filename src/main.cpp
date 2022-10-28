#include "rqs/RasterQuery.h"

using namespace RQS::structures;

int main(int argc, char *argv[]) {
    RasterQuery& rq = RasterQuery::get();
    rq.init(llPoint{41.9, -91.3});
    return 0;
}
