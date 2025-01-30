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
public:
    GameState(RenderWindow& window, Player& player);
    void handleInput() override;
    void update(float deltaTime) override;
    void draw() override;
};

#endif
