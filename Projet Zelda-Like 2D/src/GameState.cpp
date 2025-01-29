#include "../include/GameState.h"

GameState::GameState(RenderWindow& window, Player& player)
    : State(window), player(player) {
}

void GameState::handleInput() {
    player.handleInput();
}

void GameState::update(float deltaTime) {
    player.update(deltaTime, window, player.getPosition());
}

void GameState::draw() {
    player.draw(window);
}
