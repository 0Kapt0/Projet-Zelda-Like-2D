#ifndef ENTITY_H
#define ENTITY_H

#include <SFML/Graphics.hpp>
#include "Map.h"
#include <iostream>
#include <vector>

using namespace sf;
using namespace std;

class Entity {
protected:
    RectangleShape shape;
    vector<IntRect> frames;
    int currentFrame;
    float frameTime;
    float elapsedTime;
    int totalFrames;
    bool isAttacking;

public:
    Entity();
    virtual ~Entity() {}

    bool intersects(const Entity& other) const;

    void moveShape(float offsetX, float offsetY);
    void setTexture(Texture& tex, int frameWidth, int frameHeight, int totalFrames, float frameDuration);
    void animate(float deltaTime);
    void attack() { isAttacking = true; currentFrame = 0; }

    virtual void update(float deltaTime, const RenderWindow& window, const Vector2f& playerPosition, Map& map) = 0;
    virtual void draw(RenderWindow& window) = 0;
};

#endif
