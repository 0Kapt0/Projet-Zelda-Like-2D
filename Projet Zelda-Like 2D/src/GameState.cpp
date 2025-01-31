#include "../include/GameState.h"

using namespace sf;
using namespace std;


GameState::GameState(RenderWindow& window, Player& player, int gameState)
    : State(window), player(player), map("assets/maps/lobby.txt", "assets/tilesets/Tileset_Grass.png", "assets/tilesets/items.png", 32, { }), fence("assets/maps/fence.txt", "assets/tilesets/fence.png", "assets/tilesets/items.png", 32, { 2, 4 }), gameState(gameState) {
}

void GameState::handleInput() {
}

void GameState::update(float deltaTime) {
    player.update(deltaTime, window, player.getPosition(), fence);
}

void GameState::draw() {
    map.draw(window);
    fence.draw(window);
    player.draw(window);
}
