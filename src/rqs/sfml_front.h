//
// Created by quothbonney on 10/30/22.
//

#ifndef OPENGCAS_SFML_FRONT_H
#define OPENGCAS_SFML_FRONT_H

#include "rqs.h"
#include <SFML/Graphics.hpp>

namespace RQS::front {
    class dbVis {
    private:
        RQS::structures::llPoint llOrigin;
        std::array<sf::Uint8*, 9> m_db;
        std::array<sf::Texture, 9> m_tex;
        std::array<sf::Sprite, 9> m_sprite;



        double cornerLatRes;
        double cornerLonRes;

        const RQS::RasterQuery* m_rqs;
        const int w_size = 1 * 512 * 3;
        sf::RenderWindow m_window =
                sf::RenderWindow(sf::VideoMode(w_size, w_size), "Datablock Visualization");

        const int b_size = BLOCK_SIZE/2;

        auto llToPx(const RQS::structures::llPoint& loc) -> sf::Vector2f;

        void loadData();

    public:
        explicit dbVis(const RQS::RasterQuery* rqs);

        void render();
    };
}

#endif //OPENGCAS_SFML_FRONT_H
