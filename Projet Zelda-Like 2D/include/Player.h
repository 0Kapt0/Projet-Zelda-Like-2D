#ifndef PLAYER_H
#define PLAYER_H

#include "Entity.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <array>

using namespace sf;

class Player : public Entity {
private:
    Vector2f position;
    Vector2f velocity;

    int health;
    float speed;

    array<SoundBuffer, 2> swordSwingBuffers;
    array<Sound, 2> swordSwing;
    array<SoundBuffer, 6> footstepBuffers;
    array<Sound, 6> footsteps;
    Clock footstepClock;

    Texture playerRun;
    Texture playerIdle;
    Texture playerAttack1;
    Texture playerAttack2;

    View cameraView;

    bool isMoving;
public:
    Player();

    Vector2f getPosition() const;
    float getSpeed() const;
    float getHealth() const;
    void playerWalk();
    void playerAttack();
    void playFootstep();

    void setPosition(const Vector2f& newPosition);
    void setSpeed(float newSpeed);
    void reduceHealth(int damage);

    void handleInput(float deltaTime);
    void update(float deltaTime, const RenderWindow& window, const Vector2f& playerPosition) override;
    void draw(RenderWindow& window) override;

    View getCameraView() const;
};

#endif
