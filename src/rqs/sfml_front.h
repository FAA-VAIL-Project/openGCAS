//
// Created by quothbonney on 10/30/22.
//

#ifndef OPENGCAS_SFML_FRONT_H
#define OPENGCAS_SFML_FRONT_H

#include "rqs.h"
#include <SFML/Graphics.hpp>

namespace RQS::front {
    class sfmlFront {
    private:
        std::array<sf::Uint8*, 9> m_db;
        std::array<sf::Texture, 9> m_tex;
        std::array<sf::Sprite, 9> m_sprite;

        RQS::RasterQuery* m_rqs;
        sf::RenderWindow m_window =
                sf::RenderWindow(sf::VideoMode(512*3, 512*3), "SFML works!");

        const int b_size = BLOCK_SIZE/2;

        void loadData();

    public:
        explicit sfmlFront(const RQS::RasterQuery& rqs);

        void render();
    };
}

#endif //OPENGCAS_SFML_FRONT_H
