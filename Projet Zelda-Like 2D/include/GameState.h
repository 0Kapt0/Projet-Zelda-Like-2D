#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "State.h"
#include "Player.h"
#include "Merchant.h"
#include "Enemy.h"
#include "Map.h"
#include "DialogueBox.h"
#include "GameHUD.h"
#include <SFML/Graphics.hpp>
#include <vector>

using namespace sf;
using namespace std;

class GameState : public State {
private:
    Player& player;
    Merchant merchant;
    GameHUD hud;

    vector<Enemy> enemies;   // Liste des ennemis
    vector<unique_ptr<Merchant>> npcs;   // Liste des NPCs
    Clock textCD;

    Map map;
    Map fence;
    int gameState;  // 1: lobby, 2: room 1, etc...

    void spawnEnemies();
    void spawnNPCs();

public:
    GameState(RenderWindow& window, Player& player, int gameState);
    void handleInput() override;
    void update(float deltaTime) override;
    void draw() override;
};

#endif

