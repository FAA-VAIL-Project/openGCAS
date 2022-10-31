#include "rqs/rqs.h"
#include <SFML/Graphics.hpp>

using namespace RQS::structures;

int main(int argc, char *argv[]) {
    RQS::RasterQuery& rq = RQS::RasterQuery::get();
    rq.init(llPoint{41.9, -91.3});
    auto* p = rq.getDB(3)->getData();

    sf::RenderWindow window(sf::VideoMode(1024, 1024), "SFML works!");
    int W = 1024, H = 1024;
    sf::Uint8* pixels = new sf::Uint8[W*H*4];

    sf::Texture texture;
    texture.create(512, 512);

    sf::Sprite sprite(texture); // needed to draw the texture on screen

// ...

    for(int i = 0; i < W*H; i += 4) {
        int val = p[(int)std::floor(i/(512*4)) % 512][i/4 % 512]/2; // Hacky way of accessing the array;
        pixels[i] = val; // obviously, assign the values you need here to form your color
        pixels[i+1] = val;
        pixels[i+2] = val;
        pixels[i+3] = 255;
    }

    texture.update(pixels);
    texture.setRepeated(false);
    sprite.scale(sf::Vector2f(0.5f, 0.5f));

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        //window.draw(shape);
        window.draw(sprite);
        window.display();
    }
    return 0;
}
