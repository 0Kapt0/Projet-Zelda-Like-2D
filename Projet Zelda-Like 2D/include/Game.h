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
#include "VictoryScreen.h"
#include "BossEnemy.h"


using namespace sf;
using namespace std;

class Game {
private:
    RenderWindow window;
    Player player;
    unique_ptr<BossEnemy> boss;
    Texture backgroundTexture;

    unique_ptr<State> currentState;

    Clock clock;

    bool isGameOver = false;
    bool isPaused = false;
    PauseMenu pauseMenu;
    GameOverMenu gameOverMenu;
    VictoryScreen victoryScreen;
public:
    Game();
    ~Game();
    void run();
    void changeState(unique_ptr<State> newState);
    Player& getPlayer() { return player; }
    BossEnemy* getBoss() {
        return boss.get();
    }
};

#endif

