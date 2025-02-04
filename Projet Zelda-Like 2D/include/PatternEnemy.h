#ifndef PATTERN_ENEMY_H
#define PATTERN_ENEMY_H

#include "Enemy.h"
#include "Player.h"

class PatternEnemy : public Enemy {
private:
    Vector2f direction;
public:
    Player& player;
    PatternEnemy(float x, float y, float speed, float health, Player& player);

    void update(float deltaTime, const RenderWindow& window, const Vector2f& playerPosition, Map& map) override;
    void draw(RenderWindow& window) override;

    float speed;
private:
    float detectionRange;
    Texture texture;
};

#endif