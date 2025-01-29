#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "State.h"
#include "Player.h"
#include <SFML/Graphics.hpp>

class GameState : public State {
private:
    Player& player;

public:
    GameState(RenderWindow& window, Player& player);
    void handleInput() override;
    void update(float deltaTime) override;
    void draw() override;
};

#endif
