#ifndef CHASER_ENEMY_H
#define CHASER_ENEMY_H

#include "Enemy.h"
#include "Player.h"

class ChaserEnemy : public Enemy {
public:
    ChaserEnemy(float x, float y, float speed, float detectionRange, Player& _player);

    void update(float deltaTime, const RenderWindow& window, const Vector2f& playerPosition, Map& map) override;
    void draw(RenderWindow& window) override;

private:
    Player& player;
    float detectionRange;
    Texture texture;
    Texture attack;

    //Gestion de l'attaque
    bool isAttacking = false;
    Clock attackCooldown;
    float attackCooldownTime;
};

#endif
