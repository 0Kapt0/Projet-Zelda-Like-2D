#ifndef ENTITY_H
#define ENTITY_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

using namespace sf;
using namespace std;

class Entity {
protected:
    RectangleShape shape;
    Texture texture;
    vector<IntRect> frames;
    int currentFrame;
    float frameTime;
    float elapsedTime;
public:
    Entity();
    virtual ~Entity() {}

    bool intersects(const Entity& other) const;

    void setTexture(const string& textureFile, int frameWidth, int frameHeight, int totalFrames, float frameDuration);
    void animate(float deltaTime);

    virtual void update(float deltaTime, const RenderWindow& window, const Vector2f& playerPosition) = 0;
    virtual void draw(RenderWindow& window) = 0;
};

#endif

