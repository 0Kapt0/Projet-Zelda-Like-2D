#include "../include/Game.h"
#include <iostream>

using namespace sf;
using namespace std;

Game::Game()
    : window(VideoMode(920, 720), "Escape the Dungeon"),
    player() {
    window.setFramerateLimit(60);
}

void Game::run() {
    Clock clock;

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();
        }

        float updateTime = clock.restart().asSeconds();


        if (player.getHealth() <= 0) {
        }

        window.clear();
        player.draw(window);
        window.display();

    }
}