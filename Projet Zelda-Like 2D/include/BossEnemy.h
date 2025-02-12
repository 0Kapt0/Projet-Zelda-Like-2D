﻿#ifndef BOSS_ENEMY_H
#define BOSS_ENEMY_H

#include <SFML/Audio.hpp>
#include "DialogueBox.h"
#include "Enemy.h"
#include "Player.h"
#include "PatternEnemy.h"
#include "ChaserEnemy.h"
#include <vector>

using namespace sf;
using namespace std;

enum class BossPattern {
    SPAWN,
    IDLE,
    LASER,
    SUMMON,
    METEOR,
    CHARGE,
    DEAD
};

enum class BossPhase {
    INTRO_DIALOGUE,
    SPAWN,
    ATTACK_INTRO,
    IDLE,
    ATTACKING,
    DEAD
};

struct BossProjectile {
    RectangleShape shape;
    Vector2f direction;
    float speed;

    BossProjectile(const RectangleShape& _shape, Vector2f _direction, float _speed)
        : shape(_shape), direction(_direction), speed(_speed) {}

    void update(float deltaTime) {
        shape.move(direction * speed * deltaTime);
    }
};

class BossEnemy : public Enemy {
public:
    BossEnemy(float x, float y, float _detectionRange, Player& _player);

    void update(float deltaTime, const RenderWindow& window, const Vector2f& playerPosition, Map& map) override;
    void draw(RenderWindow& window) override;

    //Gestion de l'apparition et de la mort
    void startDeathAnimation();
    void startSpawnAnimation();
    void reduceHealth(float damage);

    vector<BossProjectile> projectiles;
    vector<unique_ptr<ChaserEnemy>> chaserEnemies;
    vector<unique_ptr<PatternEnemy>> patternEnemies;
    Texture laserProjectileTexture;
private:
    Player& player;
    BossPattern currentPattern;
    BossPhase currentPhase;

    float detectionRange;
    float attackCooldown;
    float attackDuration;

    Clock attackClock;
    Clock phaseClock;

    bool isAttacking;
    bool playerEnteredArena;
    bool hasDoneIntroAttack;
    bool isSpawning;
    bool isDying;

    //Barre de vie du boss
    RectangleShape bossHealthBar;
    RectangleShape bossHealthBarOutline;
    float maxHealth;
    bool bossHealthBarVisible = false;
    Text bossNameText;
    Font bossFont;
    string bossName;
    bool isDead = false;

    //Initialisation de la barre de vie
    void initializeBossHealthBar();
    void updateBossHealthBar();
    void updateBossHealthBarPosition(const View& cameraView);

    DialogueBox dialogue;
    Clock dialogueClock;
    bool waitingForNextDialogue = false;
    bool isShaking = false;
    Clock shakeClock;
    float shakeDuration = 3.f;
    void shakeDialogue();

    Music bossMusic;

    //Textures des différentes animations
    Texture idleTexture, laserTexture, summonTexture, meteorTexture, chargeTexture;
    Texture spawnTexture, deathTexture;
    Texture meteorProjectileTexture;
    Texture chargeProjectileTexture;

    //Chargement des textures
    void loadTextures();

    //Gestion des différentes phases du boss
    void checkPlayerEntry();
    void handleIntroDialogue();
    void handleSpawnAnimation();
    void handleIdlePhase();
    void handleAttackingPhase(float deltaTime);
    void handleDeathPhase();
    void checkPlayerAttack();

    Clock damageCooldown;
    float damageCooldownTime = 0.5f;

    void pushPlayerBack();
    void applySmoothPushback(float deltaTime);
    bool isPushingBack = false;
    Vector2f pushbackDirection;
    float pushbackStrength;
    Clock pushbackClock;

    RectangleShape hitbox;
    Vector2f hitboxOffset;

    void updateHitbox();
    float getBossHealth();

    //Gestion des attaques et patterns
    void changePattern();
    void executePattern(float deltaTime);
    void launchLaserAttack();
    void checkProjectileCollision();
    void launchMeteorAttack();
    void launchSummonAttack();
    void launchChargeAttack();
    /*void checkPlayerCollision();*/
};

#endif
