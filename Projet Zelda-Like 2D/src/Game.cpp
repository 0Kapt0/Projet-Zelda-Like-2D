#include "../include/Game.h"
#include <iostream>

using namespace sf;
using namespace std;

Game::Game()
    : window(VideoMode(1200, 900), "Zeldouille", Style::None),
    player(),
    pauseMenu(window),
    gameOverMenu(window),
    victoryScreen(window){
    window.setFramerateLimit(60);
    window.setVerticalSyncEnabled(true);
    currentState = make_unique<MenuState>(window);
}

Game::~Game() {
    cout << "Le jeu est détruit\n";
}

void Game::run() {
    Animation* animation = new Animation(1);
    animation->Start();
    delete animation;
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
            if (isGameOver) {
                gameOverMenu.handleInput();
            }
        }

        if (player.getHealth() <= 0 && !isGameOver) {
            isGameOver = true;
            gameOverMenu.triggerGameOver();
        }

        /*if (auto gameState = dynamic_cast<GameState*>(currentState.get())) {
            if (gameState->getBoss() && gameState->getBoss()->getBossHealth() <= 0) {
                cout << "Le Boss est mort, passage à l'écran de victoire !" << endl;
                changeState(make_unique<VictoryScreen>(window));
            }
        }*/


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

        if (isGameOver) {
            gameOverMenu.draw();

        }
        if (isPaused) {
            pauseMenu.draw(window);
        }

        if (!isGameOver) {
            window.display();
        }

    }
}





void Game::changeState(unique_ptr<State> newState) {
    currentState = move(newState);
}
