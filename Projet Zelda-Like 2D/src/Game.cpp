#include "../include/Game.h"
#include <iostream>

using namespace sf;
using namespace std;

Game::Game()
    : window(VideoMode(920, 720), "Escape the Dungeon"),
    player() {
    window.setFramerateLimit(60);
    
    //temporaire pour test
    loadBackground();

    currentState = make_unique<MenuState>(window);
}

//temporaire pour test
void Game::loadBackground() {
    if (!backgroundTexture.loadFromFile("assets/backgrounds/test.png")) {
        cerr << "Erreur de chargement du background !" << endl;
    }
    backgroundSprite.setTexture(backgroundTexture);
}

void Game::run() {
    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }
            currentState->handleInput();
        }

        float updateTime = clock.restart().asSeconds();
        currentState->update(updateTime);

        if (auto menuState = dynamic_cast<MenuState*>(currentState.get())) {
            if (menuState->selectedItemIndex == 0) {
                changeState(make_unique<GameState>(window, player));
            }
        }

        window.clear();

        //temporaire pour test
        window.draw(backgroundSprite);
        currentState->draw();
        window.display();
    }
}


void Game::changeState(unique_ptr<State> newState) {
    currentState = move(newState);
}
