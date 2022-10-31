//
// Created by quothbonney on 10/30/22.
//

#include "sfml_front.h"

using namespace RQS::front;

void dbVis::loadData() {
    for(int i = 0; i < 9; ++i) {
        auto p = m_rqs->get().getDB(i)->getData();
        m_db[i] = new sf::Uint8[b_size*2*b_size*2*4];
        m_tex[i].create(b_size, b_size);

        for(int j = 0; j < 1024*1024; j += 4) {
            int val = p[(int)std::floor(j/(1024)) % 1024][j/2 % 1024]/2; // Hacky way of accessing the array;
            m_db[i][j] = val; // obviously, assign the values you need here to form your color
            m_db[i][j+1] = val;
            m_db[i][j+2] = val;
            m_db[i][j+3] = 255;
        }

        m_tex[i].update(m_db[i]);
        m_sprite[i].setTexture(m_tex[i]);
        m_sprite[i].setScale(sf::Vector2f(0.75f, 0.75f));
        m_sprite[i].setOrigin(
                sf::Vector2f(

                        (float)-1 * (i % 3) * b_size,
                        (float)-1 * std::floor(i/3) * b_size
                        ));
    }
}

dbVis::dbVis(const RQS::RasterQuery* rqs) {
    m_rqs = rqs;
    loadData();
}

void dibVis::render() {
    while (m_window.isOpen()) {
        sf::Event event;
        while (m_window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                m_window.close();
        }

        m_window.clear();
        for(int i = 0; i < 9; ++i) {
            m_window.draw(m_sprite[i]);
        }
        m_window.display();
    }
}