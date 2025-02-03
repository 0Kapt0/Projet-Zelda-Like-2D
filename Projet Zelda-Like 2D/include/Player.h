#ifndef PLAYER_H
#define PLAYER_H

#include "Entity.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <array>

using namespace sf;

class Player : public Entity {
private:
    //Position & Mouvement
    Vector2f position;
    Vector2f velocity;
    Vector2f playerSize;
    float speed;
    bool isMoving, isDashing, canDash;

    //Vie et Mort
    int health;
    bool playerDead, isDying;

    //Potions et Cooldown
    int healthPotions;
    bool isPotionOnCooldown;
    Clock potionCooldown;

    //Sons
    array<SoundBuffer, 2> swordSwingBuffers;
    array<Sound, 2> swordSwing;
    array<SoundBuffer, 2> playerDashBuffers;
    array<Sound, 2> Dash;
    array<SoundBuffer, 6> footstepBuffers;
    array<Sound, 6> footsteps;
    Clock footstepClock, dashClock, dashCooldownClock;

    //Textures et Affichage
    Texture playerRun, playerIdle, playerDeath, playerDash, playerAttack1, playerAttack2;
    RectangleShape hitbox;
    View cameraView;

    // Fonctions internes
    void playFootstep();
    void playerDie();
    void handleDeath();

public:
    //Constructeur
    Player();

    //Getters
    Vector2f getPosition() const;
    float getSpeed() const;
    float getHealth() const;
    int getHealthPotions() const;
    bool canUsePotion() const;
    float getPotionCooldownTime() const;
    View getCameraView() const;

    //Setters & Initialisation
    void setPosition(const Vector2f& newPosition);
    void setSpeed(float newSpeed);
    void initialize(Map& map);

    //Mécaniques du joueur
    void playerWalk();
    void playerAttack();
    void dash();
    void reduceHealth(int damage);

    //Gestion des potions
    void collectHealthPotion();
    void useHealthPotion();
    void updatePotionCooldown();

    //Gestion des entrées et mise à jour
    void handleInput(float deltaTime, Map& map);
    void update(float deltaTime, const RenderWindow& window, const Vector2f& playerPosition, Map& map) override;
    void draw(RenderWindow& window) override;
};

#endif
