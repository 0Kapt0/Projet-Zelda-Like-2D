#include "../include/GameState.h"

using namespace sf;
using namespace std;


GameState::GameState(RenderWindow& window, Player& player, int gameState)
    : State(window),
    player(player),
    map("assets/maps/map.txt", "assets/tilesets/tiles.png", "assets/tilesets/items.png", 32, { 6, 99, 5 }, {}),
    fence("assets/maps/fence.txt", "assets/tilesets/fence.png", "assets/tilesets/items.png", 32, {/* 2, 4 */ }, {}),
    gameState(gameState),
    dialogue(100, 30)
{
    dialogue.setDialogue({
        "Hello there!",
        "test de la dialogue box",
        "HAHA HAAHAHAAHH AHHAHAHHAH AHAHAHAHA HAHAH AHAHAH AHAAHAHAAH HAHHAHAH HAHAHAHAH AHAHA HAHAHA"
        });
}

void GameState::handleInput() {
    if (Keyboard::isKeyPressed(Keyboard::Space) && textCD.getElapsedTime().asSeconds() > 0.2f) {
        dialogue.advanceDialogue();
        textCD.restart();
    }
}


void GameState::update(float deltaTime) {
    player.update(deltaTime, window, player.getPosition(), map);

    dialogue.setPosition(player.getPosition().x - 50, player.getPosition().y - 40);
    dialogue.update();
}

void GameState::draw() {
    map.draw(window);
    /*fence.draw(window);*/
    player.draw(window);

    dialogue.draw(window);
}
