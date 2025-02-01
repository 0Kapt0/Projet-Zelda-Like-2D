#include "../include/GameState.h"
#include "../include/Merchant.h"

using namespace sf;
using namespace std;

GameState::GameState(RenderWindow& window, Player& player, int gameState)
    : State(window),
    player(player),
    merchant(450, 190),
    map("assets/maps/dungeon.txt", "assets/tilesets/tiles.png", "assets/tilesets/items.png", 32, { 6, 99, 5 }, {62, 64}),
    fence("assets/maps/fence.txt", "assets/tilesets/fence.png", "assets/tilesets/items.png", 32, {}, {}),
    gameState(gameState)
{
    //Charge la texture AVANT de cr�er les marchands
    Merchant::loadMerchantTexture("assets/NPC/merchant.png");

    // Initialise les positions des entit�s � partir de la carte
    player.setPosition(map.getPlayerStartPosition());
    spawnEnemies();
    spawnNPCs();
}

void GameState::spawnEnemies() {
    for (const auto& pos : map.getEnemyPositions()) {
        enemies.emplace_back(pos.x, pos.y, 50.0f); // 50 = vitesse de l'ennemi
    }
}

void GameState::spawnNPCs() {
    for (const auto& pos : map.getNpcPositions()) {
        npcs.emplace_back(make_unique<Merchant>(pos.x, pos.y)); 
    }
}



void GameState::handleInput() {
    if (Keyboard::isKeyPressed(Keyboard::E) && textCD.getElapsedTime().asSeconds() > 0.2f) {
        for (auto& npc : npcs) {
            if (npc->isDialogueActive()) {
                npc->interact();
            }
        }

        if (merchant.isDialogueActive()) {
            merchant.interact();
        }

        textCD.restart();
    }
}



void GameState::update(float deltaTime) {
    player.update(deltaTime, window, player.getPosition(), map);

    for (auto& enemy : enemies) {
        enemy.update(deltaTime, window, player.getPosition(), map);
    }

    for (auto& npc : npcs) {
        npc->update(deltaTime, window, player.getPosition(), map);
        npc->checkCollisionWithPlayer(player);
    }

    merchant.update(deltaTime, window, player.getPosition(), map);
    merchant.checkCollisionWithPlayer(player);
}


void GameState::draw() {
    map.draw(window);
    player.draw(window);

    for (auto& enemy : enemies) {
        enemy.draw(window);
    }

    for (auto& npc : npcs) {
        npc->draw(window);
    }

    merchant.draw(window);
}

