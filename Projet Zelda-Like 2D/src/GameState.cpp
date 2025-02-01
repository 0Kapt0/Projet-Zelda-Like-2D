#include "../include/GameState.h"
#include "../include/Merchant.h"

using namespace sf;
using namespace std;

GameState::GameState(RenderWindow& window, Player& player, int gameState)
    : State(window),
    player(player),
    merchant(450, 190),
    map("assets/maps/map.txt", "assets/tilesets/tiles.png", "assets/tilesets/items.png", 32, { 6, 99, 5 }, {62, 64}),
    fence("assets/maps/fence.txt", "assets/tilesets/fence.png", "assets/tilesets/items.png", 32, {}, {}),
    gameState(gameState),
    hud(player)
{
    //Charge la texture AVANT de créer les marchands
    Merchant::loadMerchantTexture("assets/NPC/merchant.png");

    if (!potionTexture.loadFromFile("assets/HUD/health_potion.png")) {
        std::cerr << "Erreur de chargement de la texture des potions\n";
    }

    // Charger toutes les potions trouvées dans la carte
    for (const auto& pos : map.getPotionPositions()) {
        Sprite potionSprite;
        potionSprite.setTexture(potionTexture);
        potionSprite.setPosition(pos);
        potionSprite.setScale(1.f, 1.f);
        potions.push_back(potionSprite);
    }

    // Initialise les positions des entités à partir de la carte
    player.setPosition(map.getPlayerStartPosition());
    spawnEnemies();
    spawnNPCs();
}

void GameState::spawnEnemies() {
    for (const auto& pos : map.getEnemyPositions()) {
        enemies.emplace_back(pos.x, pos.y, 50.0f);
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
    hud.update(deltaTime);

    for (auto& enemy : enemies) {
        enemy.update(deltaTime, window, player.getPosition(), map);
    }

    for (auto& npc : npcs) {
        npc->update(deltaTime, window, player.getPosition(), map);
        npc->checkCollisionWithPlayer(player);
    }

    for (auto it = potions.begin(); it != potions.end();) {
        if (player.getShape().getGlobalBounds().intersects(it->getGlobalBounds())) {
            player.collectHealthPotion();
            it = potions.erase(it);
        }
        else {
            ++it;
        }
    }

    merchant.update(deltaTime, window, player.getPosition(), map);
    merchant.checkCollisionWithPlayer(player);
}


void GameState::draw() {
    window.setView(player.getCameraView());
    map.draw(window);
    player.draw(window);

    for (auto& enemy : enemies) {
        enemy.draw(window);
    }

    for (auto& npc : npcs) {
        npc->draw(window);
    }

    for (auto& potion : potions) {
        window.draw(potion);
    }

    View hudView(FloatRect(0, 0, window.getSize().x, window.getSize().y));
    window.setView(hudView);
    hud.draw(window);
}

