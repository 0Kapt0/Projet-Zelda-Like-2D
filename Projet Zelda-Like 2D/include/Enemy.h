#ifndef ENEMY_H
#define ENEMY_H
#include "Entity.h"
#include <SFML/Graphics.hpp>
#include <iostream>

using namespace sf;
using namespace std;

class Enemy : public Entity {
private:
    Vector2f position;
    float speed;

public:
    Enemy(float x, float y, float _speed);

    void setPosition(const Vector2f& newPosition);
    const RectangleShape& getShape() const;
    Vector2f getPosition() const;

    void update(float deltaTime, const RenderWindow& window, const Vector2f& playerPosition) override;
    void draw(RenderWindow& window) override;

    virtual ~Enemy() {}
};

#endif
