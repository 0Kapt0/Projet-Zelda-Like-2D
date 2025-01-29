#include "../include/GameState.h"

using namespace sf;
using namespace std;


GameState::GameState(RenderWindow& window, Player& player)
    : State(window), player(player), map("assets/maps/map.txt", "assets/tilesets/tiles.png", 32) {
}

void GameState::handleInput() {
    /*player.handleInput();*/
}

void GameState::update(float deltaTime) {
    player.update(deltaTime, window, player.getPosition());
}

void GameState::draw() {
    map.draw(window);
    player.draw(window);
}
