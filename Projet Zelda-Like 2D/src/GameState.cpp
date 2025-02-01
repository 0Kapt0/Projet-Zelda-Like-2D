#include "../include/GameState.h"

using namespace sf;
using namespace std;


GameState::GameState(RenderWindow& window, Player& player, int gameState)
    : State(window),
    player(player), merchant(450, 190),
    map("assets/maps/map.txt", "assets/tilesets/tiles.png", "assets/tilesets/items.png", 32, { 6, 99, 5 }, {}),
    fence("assets/maps/fence.txt", "assets/tilesets/fence.png", "assets/tilesets/items.png", 32, {/* 2, 4 */ }, {}),
    gameState(gameState)
{

}

void GameState::handleInput() {
    if (Keyboard::isKeyPressed(Keyboard::E) && textCD.getElapsedTime().asSeconds() > 0.2f) {
        if (!merchant.isDialogueFinished()) {
            merchant.advanceDialogue();
        }
        else if (merchant.isDialogueActive()) {
            merchant.setDialogueActive(false);
        }
        textCD.restart();
    }
}


void GameState::update(float deltaTime) {
    player.update(deltaTime, window, player.getPosition(), map);
    merchant.update(deltaTime, window, player.getPosition(), map);
    merchant.checkCollisionWithPlayer(player);
}

void GameState::draw() {
    map.draw(window);
    /*fence.draw(window);*/
    player.draw(window);
    merchant.draw(window);
}
