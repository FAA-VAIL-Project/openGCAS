//
// Created by quothbonney on 10/13/22.
//

#include "RasterQuery.h"

///\brief Default constructor with initializer list
rqsDataBlock::rqsDataBlock(int id) : m_id(id) {
    init();
}

///\brief Initialize memory block of size 1024x1024
void rqsDataBlock::init() {
    const int blockSize = 1024;
    _spBlock = std::make_unique<_spRow[]>(blockSize);

    for(size_t i = 0; i < blockSize; ++i) {
        _spBlock[i] = std::make_unique<int[]>(blockSize);
    }
    std::cout << "Allocated " << (sizeof(int) * blockSize*blockSize) / 8 <<
                 " bytes of memory in block with id " << m_id;
}