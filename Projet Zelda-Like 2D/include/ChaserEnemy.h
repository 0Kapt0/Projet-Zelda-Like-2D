#ifndef CHASER_ENEMY_H
#define CHASER_ENEMY_H

#include "Enemy.h"
#include "Player.h"

class ChaserEnemy : public Enemy {
public:
    ChaserEnemy(float x, float y, float speed, float detectionRange);

    void update(float deltaTime, const RenderWindow& window, const Vector2f& playerPosition, Map& map) override;
    void draw(RenderWindow& window) override;

    float speed;
private:
    float detectionRange;
    Texture texture;
};

#endif
