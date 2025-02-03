#ifndef BOSS_ENEMY_H
#define BOSS_ENEMY_H

#include "DialogueBox.h"
#include "Enemy.h"
#include "Player.h"
#include <vector>

enum class BossPattern {
    SPAWN,     // Animation d'apparition
    IDLE,      // Le boss ne fait rien
    FIREBALLS, // Tire des projectiles
    LASER,     // Tire un laser
    SUMMON,    // Invoque des ennemis
    METEOR,    // Fait tomber des météores
    CHARGE,    // Charge vers le joueur
    DEAD       // Animation de mort
};

enum class BossPhase {
    INTRO_DIALOGUE,  // Le boss parle avant de spawn
    SPAWN,           // Animation d’apparition
    ATTACK_INTRO,    // Première attaque spéciale
    IDLE,            // Attente entre les attaques
    ATTACKING,       // Attaque en cours
    DEAD             // Animation de mort
};

class BossEnemy : public Enemy {
public:
    BossEnemy(float x, float y, float _detectionRange, Player& _player);

    void update(float deltaTime, const sf::RenderWindow& window, const sf::Vector2f& playerPosition, Map& map) override;
    void draw(sf::RenderWindow& window) override;

    // Gestion de l'apparition et de la mort
    void startDeathAnimation();
    void startSpawnAnimation();

private:
    Player& player;
    BossPattern currentPattern;
    BossPhase currentPhase;

    float detectionRange;
    float attackCooldown;
    float attackDuration;

    sf::Clock attackClock;
    sf::Clock phaseClock;

    bool isAttacking;
    bool playerEnteredArena;
    bool hasDoneIntroAttack;
    bool isSpawning;
    bool isDying;

    DialogueBox dialogue;
    sf::Clock dialogueClock;
    bool waitingForNextDialogue = false;


    // Textures des différentes animations
    sf::Texture idleTexture, fireballTexture, laserTexture, summonTexture, meteorTexture, chargeTexture;
    sf::Texture spawnTexture, deathTexture;

    // Chargement des textures
    void loadTextures();

    // Gestion des différentes phases du boss
    void checkPlayerEntry();
    void handleIntroDialogue();
    void handleSpawnAnimation();
    void handleIdlePhase();
    void handleAttackingPhase(float deltaTime);
    void handleDeathPhase();

    // Gestion des attaques et patterns
    void changePattern();
    void executePattern(float deltaTime);
};

#endif // BOSS_ENEMY_H
