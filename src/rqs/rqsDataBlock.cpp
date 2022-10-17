//
// Created by quothbonney on 10/13/22.
//

#include "RasterQuery.h"
#define BLOCK_SIZE 1024

///\brief Default constructor with initializer list
rqsDataBlock::rqsDataBlock(int id, int posX, int posY,
                           RasterQuery& rq, nPoint origin) : m_id(id), m_origin(origin) {
    init();

    m_rqsDataInfo = &rq.m_unsortedDirTransform;
    m_rqsCallOrder = &rq.m_rasterCallOrder;
    std::cout << origin.x << " " << origin.y << " " << origin.r << "\n";
    readFromRaster();
}

void rqsDataBlock::readFromRaster() {
    // Possibly GNU compiler bug (???) pointer to vector is a vector?, this appears to work though.
    auto raster = m_rqsDataInfo[0][m_origin.r];

    // Check if there are intersections and read from the raster based on which intersections there are (4 options)
    bool xIntersections, yIntersections;
    xIntersections = (m_origin.x + BLOCK_SIZE > raster.r_xSize);
    yIntersections = (m_origin.y + BLOCK_SIZE > raster.r_ySize);

    int rasterIndexInCallOrder;
    for(int i = 0; i < m_rqsCallOrder->size(); ++i) {
        if(m_rqsCallOrder[0][i].index == m_origin.r) { rasterIndexInCallOrder = i; }
    }

    if(!xIntersections && !yIntersections) { // If no intersections
        for(int row = 0; row < BLOCK_SIZE; ++row) {
            // Create buffer; if the raster is undefined, the buffer is set equal to the value 0,
            // Else, the raster is read normally
            int* buffer = new int[BLOCK_SIZE];

            auto t = m_rqsCallOrder[0][rasterIndexInCallOrder];
            if(t.band == nullptr) {
                buffer[BLOCK_SIZE] = { 0 };
                std::cout << "NULL\n";
            } else {
                t.band->RasterIO(GF_Read,
                            m_origin.x, m_origin.y + row,
                            BLOCK_SIZE, 1,
                            buffer,
                            BLOCK_SIZE, 1,
                            GDT_Int32,
                            0, 0
                );
            }
            // copy data to smart pointer
            memcpy(_spBlock[row].get(), buffer, BLOCK_SIZE * sizeof(int));
            delete[] buffer;
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