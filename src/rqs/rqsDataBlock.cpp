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

    // Align the index with the actual raster of the file in the RasterCallOrder
    int rasterIndexInCallOrder;
    for(int i = 0; i < m_rqsCallOrder->size(); ++i) {
        if(m_rqsCallOrder[0][i].index == m_origin.r) { rasterIndexInCallOrder = i; break; }
    }

    if(!xIntersections && !yIntersections) { // If no intersections
        for(int row = 0; row < BLOCK_SIZE; ++row) {
            // Create buffer; if the raster is undefined, the buffer is set equal to the value 0,
            // Else, the raster is read normally
            const auto* t = &m_rqsCallOrder[0][rasterIndexInCallOrder];
            if(t->band == nullptr) {
                _spBlock[row].get()[BLOCK_SIZE] = { 0 };
                std::cout << "NULL\n";
            } else {
                t->band->RasterIO(GF_Read,
                            m_origin.x, m_origin.y + row,
                            BLOCK_SIZE, 1,
                            _spBlock[row].get(),
                            BLOCK_SIZE, 1,
                            GDT_Int32,
                            0, 0
                );
            }
        }

    } else if(xIntersections && !yIntersections) {
        int maxX = m_rqsDataInfo[0][m_origin.r].r_xSize;
        int scanSize = maxX - m_origin.x;
        const auto* t = &m_rqsCallOrder[0][rasterIndexInCallOrder];
        const auto* t2 = &m_rqsCallOrder[0][rasterIndexInCallOrder + 1]; // Raster to the right
        for(int row = 0; row < BLOCK_SIZE; ++row) {
            if(t->band == nullptr) {
                _spBlock[row].get()[scanSize] = { 0 };
                std::cout << "NULL\n";
            } else {
                t->band->RasterIO(GF_Read,
                                 m_origin.x, m_origin.y + row,
                                 scanSize, 1,
                                 _spBlock[row].get(),
                                 scanSize, 1,
                                 GDT_Int32,
                                 0, 0
                );
                t2->band->RasterIO(GF_Read,
                                  0, m_origin.y + row,
                                  BLOCK_SIZE - scanSize, 1,
                                  _spBlock[row].get() + scanSize, // Offset pointer by scansize. Surprisingly easy!
                                  BLOCK_SIZE - scanSize, 1,
                                  GDT_Int32,
                                  0, 0
                );
            }
        }

    } else if(!xIntersections && yIntersections) {
        int maxY = m_rqsDataInfo[0][m_origin.r].r_ySize;
        int scanY = maxY - m_origin.y;
        const auto* t = &m_rqsCallOrder[0][rasterIndexInCallOrder];
        const auto* t2 = &m_rqsCallOrder[0][rasterIndexInCallOrder + 3]; // Raster below origin

        for(int row = 0; row < scanY; ++row) {
            if(t->band == nullptr) {
                _spBlock[row] = { 0 };
                std::cout << "NULL\n";
            } else {
                t->band->RasterIO(GF_Read,
                                 m_origin.x, m_origin.y + row,
                                 BLOCK_SIZE, 1,
                                 _spBlock[row].get(),
                                 BLOCK_SIZE, 1,
                                 GDT_Int32,
                                 0, 0
                );
            }
        }
        for(int row2 = 0; row2 < BLOCK_SIZE - scanY; ++row2) {
            if(t2->band == nullptr) {
                _spBlock[row2] = { 0 };
                std::cout << "NULL\n";
            } else {
                t2->band->RasterIO(GF_Read,
                                 m_origin.x, 0 + row2,
                                 BLOCK_SIZE, 1,
                                 _spBlock[row2 + scanY].get(),
                                 BLOCK_SIZE, 1,
                                 GDT_Int32,
                                 0, 0
                );
            }
        }

        /*for(int i = 0; i < BLOCK_SIZE; ++i) {
            for(int j = 0; j < BLOCK_SIZE; ++j) {
                std::cout << _spBlock[i][j] << " ";
            }
            std::cout << "\n";
        }*/
    } else if(xIntersections && yIntersections) {
        int maxX = m_rqsDataInfo[0][m_origin.r].r_xSize;
        int maxY = m_rqsDataInfo[0][m_origin.r].r_ySize;
        const auto* t = &m_rqsCallOrder[0][rasterIndexInCallOrder];

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