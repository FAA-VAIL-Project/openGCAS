//
// Created by quothbonney on 10/30/22.
//

#include "sfml_front.h"

using namespace RQS::front;

void sfmlFront::loadData() {
    for(int i = 0; i < 9; ++i) {
        auto p = m_rqs->getDB(i)->getData();
        m_db[i] = new sf::Uint8[b_size*b_size*4];
        m_tex[i].create(b_size, b_size);

        for(int j = 0; i < b_size*b_size; i += 4) {
            int val = p[(int)std::floor(i/(512*4)) % 512][i/4 % 512]/2; // Hacky way of accessing the array;
            m_db[i][j] = val; // obviously, assign the values you need here to form your color
            m_db[i][j+1] = val;
            m_db[i][j+2] = val;
            m_db[i][j+3] = 255;
        }

        m_tex[i].update(m_db[i]);
        m_sprite[i].setTexture(m_tex[i]);
    }
}

sfmlFront::sfmlFront(const RQS::RasterQuery &rqs) {
    loadData();
}

void sfmlFront::render() {
    while (m_window.isOpen()) {
        sf::Event event;
        while (m_window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                m_window.close();
        }

        m_window.clear();
        m_window.draw(m_sprite[5]);
        m_window.display();
    }
}