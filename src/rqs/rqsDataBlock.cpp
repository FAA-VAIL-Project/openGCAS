//
// Created by quothbonney on 10/13/22.
//

#include "RasterQuery.h"

///\brief Default constructor with initializer list
rqsDataBlock::rqsDataBlock(int id, int posX, int posY,
                           RasterQuery& rq, nPoint origin) : m_id(id), m_origin(origin) {
    init();
    auto rqsDataInfo = &rq.m_dataDirTransform;
    auto rqsCallOrder = &rq.m_rasterCallOrder;
}

auto rqsDataBlock::getOrigin() -> nPoint {

}

///\brief Initialize memory block of size 1024x1024
void rqsDataBlock::init() {
    const int blockSize = 1024;
    _spBlock = std::make_unique<_spRow[]>(blockSize);

    for(size_t i = 0; i < blockSize; ++i) {
        _spBlock[i] = std::make_unique<int[]>(blockSize);
    }
    std::cout << "Allocated " << (sizeof(int) * blockSize*blockSize) / 8 <<
                 " bytes of memory in block with id " << m_id << std::endl;
}