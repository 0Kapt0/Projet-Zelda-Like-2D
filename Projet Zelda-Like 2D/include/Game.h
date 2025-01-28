#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <memory>
#include "Player.h"

using namespace sf;
using namespace std;

class Game {
private:
    RenderWindow window;
    Player player;

public:
    Game();
    void startGame();
    void run();
};

#endif