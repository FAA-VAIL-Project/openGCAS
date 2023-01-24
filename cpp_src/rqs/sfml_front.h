//
// Created by Jack D.V. Carson on 10/30/2022.
// Copyright (C) GNU LESSER GENERAL PUBLIC LICENSE
//

#ifndef OPENGCAS_SFML_FRONT_H
#define OPENGCAS_SFML_FRONT_H

#include "rqs.h"
#include <SFML/Graphics.hpp>



namespace RQS::front {
    class RasterBorder;

    class DBVis {
    private:
        RQS::structures::llPoint llOrigin;
        std::array<sf::Uint8*, 9> m_db;
        std::array<sf::Texture, 9> m_tex;
        std::array<sf::Sprite, 9> m_sprite;
        std::vector<sf::CircleShape> m_points;
        std::vector<RasterBorder> m_borders;

        RasterQuery::_llRes cornerRes;
        double cornerLatRes;
        double cornerLonRes;
        float m_scale;

        using m_rqs = RQS::RasterQuery;

        //const RQS::RasterQuery* m_rqs;
        const int w_size = 0.5 * 512 * 3;
        sf::RenderWindow m_window = sf::RenderWindow(sf::VideoMode(w_size, w_size), "Datablock Visualization");

        const int b_size = BLOCK_SIZE/2;

        auto inline llToPx(const RQS::structures::llPoint& loc) -> sf::Vector2f const;

        void loadData();

        friend class RasterBorder;
    public:

        explicit DBVis(float scale = 0.5);

        void loadPoints(std::vector<structures::llPoint> locs);

        void render();
    };


    class RasterBorder : public sf::Drawable {
    public:
        RasterBorder(int rasterNumber, const RasterQuery *rqs, DBVis *vis);

    private:
        const float thickness;
        std::array<sf::VertexArray, 4> m_verticies;

        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
        {
            for(int i = 0; i < 4; ++i)
                target.draw(m_verticies[i], states);
        }


    };

    //sf::RenderWindow* DBVis::m_window = new sf::RenderWindow;

}

#endif //OPENGCAS_SFML_FRONT_H
