#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <memory>
#include "Entity.h"
#include "Player.h"

using namespace sf;
using namespace std;

class Game {
private:
    RenderWindow window;
    Player player;

    Clock clock;
public:
    Game();
    void run();
};

#endif