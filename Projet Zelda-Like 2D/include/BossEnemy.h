#ifndef BOSS_ENEMY_H
#define BOSS_ENEMY_H

#include "Enemy.h"
#include "Player.h"
#include <vector>

enum class BossPattern {
    IDLE,      // Le boss ne fait rien
    FIREBALLS, // Tire des projectiles
    LASER,     // Tire un laser
    SUMMON,    // Invoque des ennemis
};

class BossEnemy : public Enemy {
public:
    BossEnemy(float x, float y, float _detectionRange, Player& _player);

    void update(float deltaTime, const sf::RenderWindow& window, const sf::Vector2f& playerPosition, Map& map) override;
    void draw(sf::RenderWindow& window) override;

private:
    Player& player;
    BossPattern currentPattern;
    float detectionRange;
    sf::Clock attackClock;
    float attackCooldown;
    bool isAttacking;

    sf::Texture idleTexture, fireballTexture, laserTexture, summonTexture;

    void changePattern();
    void executePattern(float deltaTime);
};

#endif
