#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <memory>
#include "Player.h"
#include "State.h"
#include "MenuState.h"
#include "GameState.h"
#include "Map.h"

using namespace sf;
using namespace std;

class Game {
private:
    RenderWindow window;
    Player player;
    Texture backgroundTexture;

    unique_ptr<State> currentState;

    Clock clock;

public:
    Game();
    void run();
    void changeState(unique_ptr<State> newState);
};

#endif
