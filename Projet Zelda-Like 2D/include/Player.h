#ifndef PLAYER_H
#define PLAYER_H

#include "Entity.h"
#include <SFML/Graphics.hpp>

using namespace sf;

class Player : public Entity {
private:
    Vector2f position;
    Vector2f velocity;

    int health;
    float speed;

    Texture playerRun;
    Texture playerIdle;
    Texture playerAttack1;

    View cameraView;

    bool isMoving;
    bool isAttacking = false;

public:
    Player();

    Vector2f getPosition() const;
    float getSpeed() const;
    float getHealth() const;
    void playerWalk();
    void playerAttack();

    void setPosition(const Vector2f& newPosition);
    void setSpeed(float newSpeed);
    void reduceHealth(int damage);

    void handleInput(float deltaTime);
    void update(float deltaTime, const RenderWindow& window, const Vector2f& playerPosition) override;
    void draw(RenderWindow& window) override;

    View getCameraView() const;
};

#endif
