#ifndef PATTERN_ENEMY_H
#define PATTERN_ENEMY_H

#include "Enemy.h"
#include "Player.h"

class PatternEnemy : public Enemy {
public:
    PatternEnemy(float x, float y, float speed);

    void update(float deltaTime, const RenderWindow& window, const Vector2f& playerPosition, Map& map) override;
    void draw(RenderWindow& window) override;

    float speed;
private:
    float detectionRange;
    Texture texture;
};

#endif