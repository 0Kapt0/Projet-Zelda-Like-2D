#ifndef GAMESTATE_H
#define GAMESTATE_H


#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <stdexcept>
#include "State.h"
#include "Player.h"
#include "Merchant.h"
#include "Enemy.h"
#include "ChaserEnemy.h"
#include "PatternEnemy.h"
#include "BossEnemy.h"
#include "Map.h"
#include "DialogueBox.h"
#include "GameHUD.h"
#include "GameOverMenu.h"

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
    /*vector<Enemy> enemies;*/
    vector<unique_ptr<ChaserEnemy>> chaserEnemies;
    vector<unique_ptr<PatternEnemy>> patternEnemies;
    unique_ptr<BossEnemy> boss;
    vector<Sprite> potions;
    vector<Sprite> chest_closed;
    vector<Sprite> chest_open;
    Texture potionTexture;
    Texture chestOpenTexture;
    Texture chestClosedTexture;

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
    bool lobby;
    void changeMap(const string& newMapPath, bool useAlternativeSpawn);

    // --- �tat du Jeu et Minuteur ---
    int gameState;  //1: lobby, 2: salle 1, etc...
    Clock textCD;

    // Gestion du Game Over
    bool isGameOver = false;

    // --- Fonctions internes ---
    void spawnEnemies();
    void spawnNPCs();

    Clock pClock;
public:
    GameState(RenderWindow& window, Player& player, int gameState);
    ~GameState();

    void loadFromFile(Texture texture, std::string path);
    void handleInput() override;
    void update(float deltaTime) override;
    void draw() override;

    bool getIsLoading();
    bool getisGameOver();
};

#endif


