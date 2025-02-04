#ifndef CHASER_ENEMY_H
#define CHASER_ENEMY_H

#include "Enemy.h"
#include "Player.h"

class ChaserEnemy : public Enemy {
public:
    ChaserEnemy(float x, float y, float speed, float health, float detectionRange, Player& _player);

    void update(float deltaTime, const RenderWindow& window, const Vector2f& playerPosition, Map& map) override;
    void draw(RenderWindow& window) override;

    void checkPlayerAttack();
private:
    Player& player;
    float detectionRange;
    float originalSpeed;
    float speedDuringAttack;
    Texture texture;
    Texture attack;
    Texture death;

    Clock damageCooldown;
    float damageCooldownTime = 0.5f;

    //Death handle
    void handleDeath();
    bool isDying = false;
    bool isDead = false;
    Clock deathAnimationClock;
    bool deathAnimationFinished = false;

    //Gestion de l'attaque
    bool isAttacking = false;
    Clock attackCooldown;
    float attackCooldownTime;
    Clock hitClock;
};

#endif
