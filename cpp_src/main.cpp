#include "rqs/rqs.h"
#include "rqs/sfml_front.h"

using namespace RQS::structures;

int main(int argc, char *argv[]) {
    RQS::RasterQuery& rq = RQS::RasterQuery::get();
    rq.init(llPoint{41.9, -89.9 -1.2});
    RQS::front::DBVis vis;
    std::vector<llPoint> v{
        llPoint{42.1, -91.4}
    };
    vis.loadPoints(v);
    vis.render();
    return 0;
}
