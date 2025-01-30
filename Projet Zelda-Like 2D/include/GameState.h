#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "State.h"
#include "Player.h"
#include "Map.h"
#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;


class GameState : public State {
private:
    Player& player;

    Map map;
    Map fence;
    int gameState;  //1 : lobby, 2: room 1, etc...
public:
    GameState(RenderWindow& window, Player& player, int gameState);
    void handleInput() override;
    void update(float deltaTime) override;
    void draw() override;
};

#endif
