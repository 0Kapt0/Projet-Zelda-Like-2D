#ifndef PATTERN_ENEMY_H
#define PATTERN_ENEMY_H

#include "Enemy.h"
#include "Player.h"

class PatternEnemy : public Enemy {
private:
    Vector2f direction;
public:
    Player& player;
    PatternEnemy(float x, float y, float speed, float health, float detectionRange, Player& player);

    void update(float deltaTime, const RenderWindow& window, const Vector2f& playerPosition, Map& map) override;
    void draw(RenderWindow& window) override;

    void checkPlayerAttack();
    float speed;
private:
    float detectionRange;
    Texture texture;
    float originalSpeed;
    float speedDuringAttack;
    Texture attack;
    Texture death;

    Clock damageCooldown;
    float damageCooldownTime = 0.5f;

    void handleDeath();
    bool isDying = false;
    bool isDead = false;
    Clock deathAnimationClock;
    bool deathAnimationFinished = false;

    bool isAttacking = false;
    Clock attackCooldown;
    float attackCooldownTime;
    Clock hitClock;

    Clock spawnClock;
    float spawnDelay = 0.1f;
    bool isSpawning = true;
};

#endif