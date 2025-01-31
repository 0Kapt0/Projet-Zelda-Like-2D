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
    array<SoundBuffer, 1> playerDashBuffers;
    array<Sound, 1> playerDash;
    array<SoundBuffer, 6> footstepBuffers;
    array<Sound, 6> footsteps;
    Clock footstepClock;
    Clock dashClock;

    Texture playerRun;
    Texture playerIdle;
    Texture playerDeath;
    Texture playerDash;
    Texture playerAttack1;
    Texture playerAttack2;

    View cameraView;

    bool isMoving;
    bool isDying;
    bool playerDead;
    void playFootstep();
    void playerDie();
    void handleDeath();
public:
    Player();

    Vector2f getPosition() const;
    float getSpeed() const;
    float getHealth() const;
    void playerWalk();
    void playerAttack();
    void dash();
    bool isDashing;


    void setPosition(const Vector2f& newPosition);
    void setSpeed(float newSpeed);
    void reduceHealth(int damage);

    void handleInput(float deltaTime);
    void update(float deltaTime, const RenderWindow& window, const Vector2f& playerPosition) override;
    void draw(RenderWindow& window) override;

    View getCameraView() const;
};

#endif
