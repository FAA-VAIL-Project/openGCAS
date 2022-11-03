//
// Created by Jack D.V. Carson on 10/30/2022.
// Copyright (C) GNU LESSER GENERAL PUBLIC LICENSE
//

#include "sfml_front.h"

using namespace RQS::front;
using namespace RQS::structures;

DBVis::DBVis(const RQS::RasterQuery* rqs) {
    m_rqs = rqs;
    llOrigin = rqs->get().toLL(rqs->get().getDB(0)->m_origin);
    auto tmp = rqs->get()
            .getDataTransform()
            [m_rqs->get().getDB(4)->m_origin.r];
    cornerLatRes = tmp.lat_res;
    cornerLonRes = tmp.lon_res;
    std::cout << llOrigin.lat << " " << llOrigin.lon;
    loadData();
}

void DBVis::loadData() {
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
        m_sprite[i].setScale(sf::Vector2f(0.5f, 0.5f));
        m_sprite[i].setOrigin(
                sf::Vector2f(

                        (float)-1 * (i % 3) * b_size,
                        (float)-1 * std::floor(i/3) * b_size
                ));
    }
}

auto inline DBVis::llToPx(const RQS::structures::llPoint& loc) -> sf::Vector2f const {
    float x = ((loc.lat - llOrigin.lat)/cornerLatRes) / 4;
    float y = ((loc.lon - llOrigin.lon)/cornerLonRes) / 4;
    std::cout << "\nllToPx(): " << x << " " << y;
    return sf::Vector2f{1*y, 1*x};
}

void DBVis::render() {
    auto n = llToPx(llPoint{-89.4046,41.2921}.invert());
    sf::CircleShape shape(5);
    sf::Vertex point(n, sf::Color::Green);
    shape.setOrigin(n);

    RasterBorder rb(5, m_rqs, this);
// set the shape color to green
    shape.setFillColor(sf::Color(100, 250, 50));

    while (m_window.isOpen()) {
        sf::Event event;
        while (m_window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                m_window.close();
        }

        m_window.clear();
        for(int i = 0; i < 9; ++i) {
            m_window.draw(m_sprite[i]);
            m_window.draw(rb);
        }
        m_window.draw(shape);
        m_window.display();
    }
}

RasterBorder::RasterBorder(int rasterNumber, const RasterQuery *rqs, DBVis *vis)
: thickness(5.f) {
    auto dat = rqs->get().getDataTransform()[rasterNumber];

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