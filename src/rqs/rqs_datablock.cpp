//
// Created by Jack D.V. Carson on 10/13/2022.
// Copyright (C) GNU LESSER GENERAL PUBLIC LICENSE
//

#include "rqs.h"

using namespace RQS::structures;
using namespace RQS;

///\brief Default constructor with initializer list
rqsDataBlock::rqsDataBlock(int id, int posX, int posY,
                           RasterQuery& rq, nPoint origin,
                           llPoint llOrigin)
                           : m_id(id), m_origin(origin), m_llOrigin(llOrigin) {
    if(abs(posY) > 1 || abs(posX) > 1) {
        std::stringstream s;
        s << "Block position " << posX << " " << posY << " lies beyond defined raster bounds (-1, 1), (-1, 1)"
          << " for raster with size 9";
        throw std::invalid_argument(s.str());
    }

    init();
    m_rqsDataInfo = &rq.m_dataDirTransform;
    m_rqsCallOrder = &rq.m_rasterCallOrder;
    readFromRaster();
    std::cout << "Raster Origin: " << m_origin.x << " " << m_origin.y << " " << m_origin.r << "\n";
    for(int i = 0; i < m_rqsCallOrder->size(); ++i) {
        if(std::get<1>(m_rqsCallOrder[0][i]) == m_origin.r) { std::cout<< "Found Index "<< i << "\n" ; break; }
    }
    //debugWriteBitmap();
}

void rqsDataBlock::readFromRaster() {
    // Possibly GNU compiler bug (???) pointer to vector is a vector?, this appears to work though.
    auto raster = m_rqsDataInfo[0][m_origin.r];


    // Check if there are intersections and read from the raster based on which intersections there are (4 options)
    bool xIntersections, yIntersections;
    xIntersections = (m_origin.x + BLOCK_SIZE > raster.r_xSize);
    yIntersections = (m_origin.y + BLOCK_SIZE > raster.r_ySize);

#ifdef __DEBUG_VERBOSE
    std::cout << "Raster Origin: " << m_origin.x << " " << m_origin.y << " " << m_origin.r << "\n";
    std::cout << (xIntersections ? "XINT " : "");
    std::cout << (yIntersections ? "YINT\n" : " \n");
#endif

    // Align the index with the actual raster of the file in the RasterCallOrder
    int rasterIndexInCallOrder;
    for(int i = 0; i < m_rqsCallOrder->size(); ++i) {
        if(std::get<1>(m_rqsCallOrder[0][i]) == m_origin.r) { rasterIndexInCallOrder = i; break; }
    }

    if(!xIntersections && !yIntersections) { // If no intersections
        for(int row = 0; row < BLOCK_SIZE; ++row) {
            // Create buffer; if the raster is undefined, the buffer is set equal to the value 0,
            // Else, the raster is read normally
            const auto* t = &m_rqsCallOrder[0][rasterIndexInCallOrder];
            if(std::get<0>(*t) == nullptr) {
                _spBlock[row].get()[BLOCK_SIZE] = { 0 };
            } else {
                std::get<0>(*t)->RasterIO(GF_Read,
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
        int scanX = maxX - m_origin.x;
        const auto* t = &m_rqsCallOrder[0][rasterIndexInCallOrder];
        const auto* t2 = &m_rqsCallOrder[0][rasterIndexInCallOrder + 1]; // Raster to the right
        for(int row = 0; row < BLOCK_SIZE; ++row) {
            if(std::get<0>(*t)== nullptr) {
                _spBlock[row].get()[scanX] = { 0 };
                continue;
            }
            if(std::get<0>(*t2) == nullptr) {
                (_spBlock[row].get() + scanX)[BLOCK_SIZE - scanX] = { 0 };
                continue;
            }
            else {
                std::get<0>(*t)->RasterIO(GF_Read, m_origin.x, m_origin.y + row, scanX, 1,
                                  _spBlock[row].get(),
                                  scanX, 1, GDT_Int32,0, 0
                );
                std::get<0>(*t2)->RasterIO(GF_Read, 0, m_origin.y + row, BLOCK_SIZE - scanX, 1,
                                   _spBlock[row].get() + scanX, // Offset pointer by scansize. Surprisingly easy!
                                   BLOCK_SIZE - scanX, 1, GDT_Int32, 0, 0
                );
            }
        }

    } else if(!xIntersections && yIntersections) {
        int maxY = m_rqsDataInfo[0][m_origin.r].r_ySize;
        int scanY = maxY - m_origin.y;
        const auto* t = &m_rqsCallOrder[0][rasterIndexInCallOrder];
        const auto* t2 = &m_rqsCallOrder[0][rasterIndexInCallOrder + 3]; // Raster below origin

        for(int row = 0; row < scanY; ++row) {
            if(std::get<0>(*t) == nullptr) {
                _spBlock[row].get()[BLOCK_SIZE] = { 0 };
                continue;
            }

            else {
                std::get<0>(*t)->RasterIO(GF_Read,
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
            if(std::get<0>(*t2) == nullptr) {
                _spBlock[scanY + row2].get()[BLOCK_SIZE] = { 0 };
            } else {
                std::get<0>(*t2)->RasterIO(GF_Read,
                                 m_origin.x, 0 + row2,
                                 BLOCK_SIZE, 1,
                                 _spBlock[row2 + scanY].get(),
                                 BLOCK_SIZE, 1,
                                 GDT_Int32,
                                 0, 0
                );
            }
        }

    } else if(xIntersections && yIntersections) {
        int maxX = m_rqsDataInfo[0][m_origin.r].r_xSize;
        int maxY = m_rqsDataInfo[0][m_origin.r].r_ySize;
        const auto* tOrigin = &m_rqsCallOrder[0][rasterIndexInCallOrder];
        const auto* tX = &m_rqsCallOrder[0][rasterIndexInCallOrder + 1];
        const auto* tY = &m_rqsCallOrder[0][rasterIndexInCallOrder + 3];
        const auto* tXY = &m_rqsCallOrder[0][rasterIndexInCallOrder + 4];

        int scanX = maxX - m_origin.x;
        int scanY = maxY - m_origin.y;

        for(int row = 0; row < scanY; ++row) {
            if(std::get<0>(*tOrigin)== nullptr) {
                _spBlock[row].get()[scanX] = { 0 };
            } if(std::get<0>(*tX) == nullptr) {
                (_spBlock[row].get() + scanX)[BLOCK_SIZE - scanX] = { 0 };
            }
            else {
                std::get<0>(*tOrigin)->RasterIO(GF_Read,
                                  m_origin.x, m_origin.y + row,
                                  scanX, 1,
                                  _spBlock[row].get(),
                                  scanX, 1,
                                  GDT_Int32,
                                  0, 0
                );
                std::get<0>(*tX)->RasterIO(GF_Read,
                                   0, m_origin.y + row,
                                   BLOCK_SIZE - scanX, 1,
                                   _spBlock[row].get() + scanX, // Offset pointer by scansize. Surprisingly easy!
                                   BLOCK_SIZE - scanX, 1,
                                   GDT_Int32,
                                   0, 0
                );
            }
        }

        for(int row2 = scanY; row2 < BLOCK_SIZE; ++row2) {
            if(std::get<0>(*tY) == nullptr) {
                _spBlock[scanY + row2].get()[scanX] = { 0 };
            } if(std::get<0>(*tXY)== nullptr) {
                (_spBlock[scanY + row2].get() + scanX)[BLOCK_SIZE - scanX] = { 0 };
            } else {
                std::get<0>(*tY)->RasterIO(GF_Read,
                                        m_origin.x, row2 - scanY,
                                        scanX, 1,
                                        _spBlock[row2].get(),
                                        scanX, 1,
                                        GDT_Int32,
                                        0, 0
                );
                std::get<0>(*tXY)->RasterIO(GF_Read,
                                   0, row2 - scanY,
                                   BLOCK_SIZE - scanX, 1,
                                   _spBlock[row2].get() + scanX, // Offset pointer by scansize. Surprisingly easy!
                                   BLOCK_SIZE - scanX, 1,
                                   GDT_Int32,
                                   0, 0
                );
            }
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
#ifdef __DEBUG_VERBOSE
    std::cout << "Allocated " << (sizeof(int) * blockSize*blockSize) / 8 <<
                 " bytes of memory in block with id " << m_id << std::endl;
#endif
}

void rqsDataBlock::debugWriteBitmap() {
    std::ofstream bitmap;
    std::stringstream filename;
    filename << "../etc/output_vis/" << std::to_string(m_id) << "bitmap.pgm";
    bitmap.open(filename.str());
    bitmap << "P2\n" << BLOCK_SIZE << ' ' << BLOCK_SIZE << "\n255\n";
    for(int i = 0; i < BLOCK_SIZE; ++i) {
        for(int j = 0; j < BLOCK_SIZE; ++j) {
            int adj_int = (_spBlock[i][j] / 5) % 255;
            bitmap << adj_int << "  ";
        }
        bitmap << "\n";
    }
    std::stringstream command;
}

void rqsDataBlock::readRasterFromTuple(int rasterIndex,
                                       std::tuple<structures::nPoint, structures::nPoint> nLocs,
                                       structures::nPoint blockIndex) {
    assert(std::get<0>(nLocs).r == std::get<1>(nLocs).r);
    int scanY = std::get<1>(nLocs).y - std::get<0>(nLocs).y;
    int scanX = std::get<1>(nLocs).x - std::get<0>(nLocs).x;

    for(int row = 0; row < scanY; ++row) {
        std::get<0>(m_rqsCallOrder[0][rasterIndex])->RasterIO(GF_Read,
                                                      std::get<0>(nLocs).x,
                                                      std::get<0>(nLocs).y + row,
                                                      scanX, 1,
                                                      _spBlock[row + blockIndex.y].get() + blockIndex.x,
                                                      scanX, 1, GDT_Int32,
                                                      0, 0
                                                      );
    }
}

void rqsDataBlock::n_readFromRaster() {

}

auto RasterQuery::defineLLRes(llPoint loc) -> std::tuple<double, double, int> {

    auto sort_cmp = [&](RasterQuery::_rb_tup const & x,
                        RasterQuery::_rb_tup const & y) -> bool
    { return std::get<1>(x) < std::get<1>(y); };
    auto l_callOrder = m_rasterCallOrder;

    std::sort(std::begin(l_callOrder), std::end(l_callOrder),sort_cmp);

    int c = RQS::RasterQuery::get().getClosest(loc);
    auto tmp = std::lower_bound(l_callOrder.begin(), l_callOrder.end(), c,
                                [](RasterQuery::_rb_tup const & lhs,  int target) -> bool
                                { return std::get<1>(lhs) < target; });
    int index = std::get<1>(*tmp);
    return std::make_tuple(m_dataDirTransform[index].lat_res, m_dataDirTransform[index].lon_res, index);
}
