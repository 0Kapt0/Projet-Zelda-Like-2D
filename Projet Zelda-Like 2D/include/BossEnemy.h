#ifndef BOSS_ENEMY_H
#define BOSS_ENEMY_H

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

class BossEnemy : public Enemy {
public:
    BossEnemy(float x, float y, float _detectionRange, Player& _player);

    void update(float deltaTime, const sf::RenderWindow& window, const sf::Vector2f& playerPosition, Map& map) override;
    void draw(sf::RenderWindow& window) override;

    void startDeathAnimation();
    void startSpawnAnimation();

private:
    Player& player;
    BossPattern currentPattern;
    float detectionRange;
    sf::Clock attackClock;
    float attackCooldown;
    bool isAttacking;

    float attackDuration;  // Durée de l'attaque en secondes
    bool isSpawning;
    bool isDying;

    // Textures des différentes animations
    sf::Texture idleTexture, fireballTexture, laserTexture, summonTexture, meteorTexture, chargeTexture;
    sf::Texture spawnTexture, deathTexture;

    void changePattern();
    void executePattern(float deltaTime);
};

#endif
