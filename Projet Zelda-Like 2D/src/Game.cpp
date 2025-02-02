﻿#include "../include/Game.h"
#include <iostream>

using namespace sf;
using namespace std;

Game::Game()
    : window(VideoMode(1200, 900), "projet zelda 2D"),
    player(),
    pauseMenu(window) {
    window.setFramerateLimit(60);

    currentState = make_unique<MenuState>(window);
}

void Game::run() {
    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }

            if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape) {
                isPaused = !isPaused;
            }

            if (isPaused) {
                pauseMenu.handleInput(window, isPaused);
            }
            else {
                currentState->handleInput();
            }
        }

        float updateTime = clock.restart().asSeconds();

        if (!isPaused) {
            currentState->update(updateTime);

            if (auto menuState = dynamic_cast<MenuState*>(currentState.get())) {
                if (menuState->selectedItemIndex == 0) {
                    changeState(make_unique<GameState>(window, player, 1));
                }
            }
        }

        window.clear();

        currentState->draw();

        if (isPaused) {
            pauseMenu.draw(window);
        }

        window.display();
    }
}



void Game::changeState(unique_ptr<State> newState) {
    currentState = move(newState);
}
