#include "rqs/rqs.h"
#include "rqs/sfml_front.h"
#include <SFML/Graphics.hpp>

using namespace RQS::structures;

int main(int argc, char *argv[]) {
    RQS::RasterQuery& rq = RQS::RasterQuery::get();
    rq.init(llPoint{41.9, -91.3});
    RQS::front::dbVis vis(&rq);
    vis.render();
    return 0;
}
