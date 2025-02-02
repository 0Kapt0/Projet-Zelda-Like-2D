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
#include <SFML/Audio.hpp>
#include <vector>

using namespace sf;
using namespace std;

class GameState : public State {
private:
    // --- Joueur et Interface ---
    Player& player;
    GameHUD hud;

    // --- NPCs et Marchands ---
    Merchant merchant;
    vector<unique_ptr<Merchant>> npcs;

    // --- Ennemis et Potions ---
    vector<Enemy> enemies;
    vector<Sprite> potions;
    Texture potionTexture;

    // --- Transition fondu ---
    RectangleShape fadeOverlay;
    float fadeAlpha = 0.0f;
    bool isFading = false;
    bool fadeIn = false;
    bool isLoading = false;
    void updateFade(float deltaTime);

    // --- Sound ---
    Music ambientSound;

    // --- Carte et Environnement ---
    Map map;
    Map fence;
    void changeMap(const string& newMapPath);

    // --- État du Jeu et Minuteur ---
    int gameState;  //1: lobby, 2: salle 1, etc...
    Clock textCD;

    // --- Fonctions internes ---
    void spawnEnemies();
    void spawnNPCs();

public:
    GameState(RenderWindow& window, Player& player, int gameState);
    ~GameState();

    void handleInput() override;
    void update(float deltaTime) override;
    void draw() override;
};

#endif


