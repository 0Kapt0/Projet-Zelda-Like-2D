#ifndef PLAYER_H
#define PLAYER_H

#include "Entity.h"
#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;

class Player : public Entity {
private:
    Vector2f position;
    int health;
    float speed;
    Texture playerTexture;

    void handleInput();

public:
    Player();

    Vector2f getPosition() const;
    float getSpeed() const;
    float getHealth() const;

    void setPosition(const Vector2f& newPosition);
    void reduceHealth(int damage);
    void setSpeed(float newSpeed);

    void update(float deltaTime, const RenderWindow& window, const Vector2f& playerPosition) override;
    void draw(RenderWindow& window) override;
};

#endif
