#include "rqs/rqs.h"
#include "rqs/sfml_front.h"

using namespace RQS::structures;

int main(int argc, char *argv[]) {
    RQS::RasterQuery& rq = RQS::RasterQuery::get();
    rq.init(llPoint{41.9, -89.9});
    RQS::front::DBVis vis(&rq);
    vis.render();
    return 0;
}
