//
// Created by Jack D.V. Carson on 10/30/2022.
// Copyright (C) GNU LESSER GENERAL PUBLIC LICENSE
//

#include "sfml_front.h"

using namespace RQS::front;
using namespace RQS::structures;

DBVis::DBVis(float scale) : m_scale(scale) {
    llOrigin = m_rqs::get().toLL(m_rqs::get().getDB(0)->m_origin);
    auto tmp = m_rqs::get()
            .getDataTransform()
            [m_rqs::get().getDB(4)->m_origin.r];
    cornerLatRes = tmp.lat_res;
    cornerLonRes = tmp.lon_res;
    std::cout << llOrigin.lat << " " << llOrigin.lon;
    loadData();
}

void DBVis::loadData() {

    auto dat = m_rqs::get().getDataTransform();
    auto calls = m_rqs::get().getCallOrder();
    for(int i = 0; i < 9; ++i) {
        m_borders.push_back(static_cast<RasterBorder>(RasterBorder(std::max(std::get<1>(calls[i]), 0), &m_rqs::get(), this)));

        auto rDB = m_rqs::get().getDB(i);
        auto p = rDB->getData();
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
        auto t = m_db[i];
        m_sprite[i].setOrigin(
                sf::Vector2f(

                        (float)-1 * (i % 3) * m_scale * 1024,
                        (float)-1 * std::floor(i/3) * m_scale * 1024
                ));
        m_sprite[i].setScale(sf::Vector2f(m_scale, m_scale));
    }
}

auto inline DBVis::llToPx(const RQS::structures::llPoint& loc) -> sf::Vector2f const {
    float x = ((loc.lat - llOrigin.lat)/cornerLatRes) * (m_scale*m_scale);
    float y = ((loc.lon - llOrigin.lon)/cornerLonRes) * (m_scale*m_scale);
    std::cout << "\nllToPx(): " << x << " " << y;
    return sf::Vector2f{1*y, 1*x};
}

void DBVis::render() {
// set the shape color to green

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
        for(const auto& b : m_borders)
            m_window.draw(b);

        for( const auto& p : m_points)
            m_window.draw(p);
        m_window.display();
    }
}

void DBVis::loadPoints(std::vector<llPoint> locs) {
    for(const auto& ll : locs) {
        auto tmp = llToPx(ll);
        auto n = sf::Vector2f{tmp.x*-1, tmp.y*-1};
        sf::CircleShape shape(5);
        sf::Vertex point(n, sf::Color::Green);
        shape.setOrigin(n);
        shape.setFillColor(sf::Color(100, 250, 50));

        m_points.push_back(shape);
    }
}


RasterBorder::RasterBorder(int rasterNumber, const RasterQuery *rqs, DBVis *vis)
: thickness(3.f) {
    auto dat = RQS::RasterQuery::get().getDataTransform()[rasterNumber];

    llPoint min = llPoint{dat.lat_o, dat.lon_o};
    llPoint max = dat.maxLL();
    sf::Vector2f pos[4];

    pos[0] = sf::Vector2f(vis->llToPx(min));
    pos[1] = sf::Vector2f(vis->llToPx(llPoint{max.lat, min.lon}));
    pos[2] = sf::Vector2f(vis->llToPx(max));
    pos[3] = sf::Vector2f(vis->llToPx(llPoint{min.lat, max.lon}));

    for(int i = 0; i < 4; ++i) {
        sf::Vector2f direction = pos[(i + 1)%4] - pos[i];
        sf::Vector2f unitDirection = direction/std::sqrt(direction.x*direction.x+direction.y*direction.y);
        sf::Vector2f unitPerpendicular(-unitDirection.y,unitDirection.x);

        sf::Vector2f offset = (thickness/2.f)*unitPerpendicular;

        auto vertex = sf::VertexArray(sf::Quads, 4);
        vertex[0].position = pos[i] + offset;
        vertex[1].position = pos[(i + 1) % 4] + offset;
        vertex[2].position = pos[(i + 1) % 4] - offset;
        vertex[3].position = pos[i] - offset;

        vertex[0].color = sf::Color::Green;
        vertex[1].color = sf::Color::Green;
        vertex[2].color = sf::Color::Green;
        vertex[3].color = sf::Color::Green;

        m_verticies[i] = vertex;
    }

}