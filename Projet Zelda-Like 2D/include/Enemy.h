#ifndef ENEMY_H
#define ENEMY_H
#include "Entity.h"
#include <SFML/Graphics.hpp>
#include <iostream>

using namespace sf;
using namespace std;

class Enemy : public Entity {
protected:
    Vector2f position;

    RectangleShape shape;
    Texture texture;

    float speed;

    IntRect frameRect;
    float animationTimer;
    float frameDuration;
    int currentFrame;
    int totalFrames;

public:
    Enemy(float x, float y, float speed);

    void setPosition(const Vector2f& newPosition);

    const RectangleShape& getShape() const;
    Vector2f getPosition() const;

    void update(float deltaTime, const RenderWindow& window, const Vector2f& playerPosition) override;
    void draw(RenderWindow& window) override;

    virtual ~Enemy() {}
};

#endif
