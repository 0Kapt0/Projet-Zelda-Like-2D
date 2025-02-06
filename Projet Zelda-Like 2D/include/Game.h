#pragma once
#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <memory>
#include "Player.h"
#include "State.h"
#include "MenuState.h"
#include "GameState.h"
#include "Map.h"
#include "PauseMenu.h"
#include "GameOverMenu.h"
#include "Animation.h"

using namespace sf;
using namespace std;

class Game {
private:
    RenderWindow window;
    Player player;
    Texture backgroundTexture;

    unique_ptr<State> currentState;

    Clock clock;

    bool isGameOver = false;
    bool isPaused = false;
    PauseMenu pauseMenu;
    GameOverMenu gameOverMenu;
public:
    Game();
    ~Game();
    void run();
    void changeState(unique_ptr<State> newState);
    Player& getPlayer() { return player; }
};

#endif

