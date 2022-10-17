//
// Created by quothbonney on 10/13/22.
//

#include "RasterQuery.h"
#define BLOCK_SIZE 1024

///\brief Default constructor with initializer list
rqsDataBlock::rqsDataBlock(int id, int posX, int posY,
                           RasterQuery& rq, nPoint origin) : m_id(id), m_origin(origin) {
    init();

    m_rqsDataInfo = &rq.m_dataDirTransform;
    m_rqsCallOrder = &rq.m_rasterCallOrder;
    std::cout << origin.x << " " << origin.y << " " << origin.r << "\n";
    readFromRaster();
}

void rqsDataBlock::readFromRaster() {
    // Possibly GNU compiler bug (???) pointer to vector is a vector?, this appears to work though.
    RasterQuery::geoTransformData raster = m_rqsDataInfo[0][m_origin.r];
    bool xIntersections, yIntersections;

    xIntersections = (m_origin.x + BLOCK_SIZE < raster.r_xSize);
    yIntersections = (m_origin.y + BLOCK_SIZE < raster.r_ySize);

    if(!xIntersections && !yIntersections) {
        for(int row = 0; row < BLOCK_SIZE; ++row) {
            m_rqsCallOrder[0][m_origin.r]->RasterIO(GF_Read,
                                                    m_origin.x, m_origin.y + row,
                                                    BLOCK_SIZE, 1,
                                                    &_spBlock[row],
                                                    BLOCK_SIZE, 1,
                                                    GDT_Int32,
                                                    0, 0
            );
        }
    }
}

///\brief Initialize memory block of size 1024x1024
void rqsDataBlock::init() {
    const int blockSize = BLOCK_SIZE;
    _spBlock = std::make_unique<_spRow[]>(blockSize);

    for(size_t i = 0; i < blockSize; ++i) {
        _spBlock[i] = std::make_unique<int[]>(blockSize);
    }
    std::cout << "Allocated " << (sizeof(int) * blockSize*blockSize) / 8 <<
                 " bytes of memory in block with id " << m_id << std::endl;
}