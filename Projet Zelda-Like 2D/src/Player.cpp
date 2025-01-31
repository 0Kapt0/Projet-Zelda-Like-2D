#include "../include/Player.h"
#include "../include/Map.h"
#include <iostream>

using namespace sf;
using namespace std;

Player::Player() : speed(150.0f), position(400.0f, 400.0f), health(100), playerDead(false), isDying(false), isDashing(false), canDash(true), isMoving(false) {
    array<string, 6> footstepFiles = {
        "assets/player/sounds/footstep1.wav",
        "assets/player/sounds/footstep2.wav",
        "assets/player/sounds/footstep3.wav",
        "assets/player/sounds/footstep4.wav",
        "assets/player/sounds/footstep5.wav",
        "assets/player/sounds/footstep6.wav"
    };
    for (size_t i = 0; i < footstepFiles.size(); i++) {
        if (!footstepBuffers[i].loadFromFile(footstepFiles[i])) {
            cerr << "Erreur de chargement : " << footstepFiles[i] << endl;
        }
        footsteps[i].setBuffer(footstepBuffers[i]);
        footsteps[i].setVolume(20.f);
    }
    array<string, 2> swordSwingFiles = {
        "assets/player/sounds/swordSwing1.wav",
        "assets/player/sounds/swordSwing2.wav"
    };
    for (size_t i = 0; i < swordSwingFiles.size(); i++) {
        if (!swordSwingBuffers[i].loadFromFile(swordSwingFiles[i])) {
            cerr << "Erreur de chargement : " << swordSwingFiles[i] << endl;
        }
        swordSwing[i].setBuffer(swordSwingBuffers[i]);
        swordSwing[i].setVolume(80.f);
    }
    array<string, 2> DashFiles = {
        "assets/player/sounds/playerDash1.wav",
        "assets/player/sounds/playerDash2.wav"
    };
    for (size_t i = 0; i < DashFiles.size(); i++) {
        if (!playerDashBuffers[i].loadFromFile(DashFiles[i])) {
            cerr << "Erreur de chargement : " << DashFiles[i] << endl;
        }
        Dash[i].setBuffer(playerDashBuffers[i]);
        Dash[i].setVolume(20.f);
    }
    if (!playerRun.loadFromFile("assets/player/player_run.png")) {
        cerr << "Texture player_run loaded!" << endl;
    }
    else {
        cout << "Texture player_run loaded" << endl;
    }
    if (!playerIdle.loadFromFile("assets/player/player_idle.png")) {
        cerr << "Texture player_idle not loaded!" << endl;
    }
    else {
        cout << "Texture player_idle loaded" << endl;
    }
    if (!playerDeath.loadFromFile("assets/player/player_death.png")) {
        cerr << "Texture player_death not loaded!" << endl;
    }
    else {
        cout << "Texture player_death loaded" << endl;
    }
    if (!playerDash.loadFromFile("assets/player/player_dash.png")) {
        cerr << "Texture player_dash not loaded!" << endl;
    }
    else {
        cout << "Texture player_dash loaded" << endl;
    }
    if (!playerAttack1.loadFromFile("assets/player/player_attack1.png")) {
        cerr << "Texture player_attack1 not loaded!" << endl;
    }
    else {
        cout << "Texture player_attack1 loaded" << endl;
    }

    playerSize = { 16, 28 };
    moveShape(8, 14);
    hitbox.setSize({ 16, 28 });
    hitbox.setOrigin(0, 0);
    hitbox.setFillColor(Color::Color(255, 0, 0, 150));

    cameraView.setSize(426.67f, 320);
    cameraView.setCenter(position);
}

void Player::handleInput(float deltaTime, Map& map) {
    Vector2f mouv(0.0f, 0.0f);

    if (!isDying) {
        if (Keyboard::isKeyPressed(Keyboard::Q) || Keyboard::isKeyPressed(Keyboard::A)) mouv.x -= 1.0f, shape.setScale(-1, 1);
        if (Keyboard::isKeyPressed(Keyboard::D)) mouv.x += 1.0f, shape.setScale(1, 1);
        if (Keyboard::isKeyPressed(Keyboard::Z) || Keyboard::isKeyPressed(Keyboard::W)) mouv.y -= 1.0f;
        if (Keyboard::isKeyPressed(Keyboard::S)) mouv.y += 1.0f;
        if (Keyboard::isKeyPressed(Keyboard::R)) reduceHealth(10);
    }

    if (mouv.x != 0 || mouv.y != 0) {
        float length = sqrt(mouv.x * mouv.x + mouv.y * mouv.y);
        mouv /= length;
        isMoving = true;
    }
    else
    {
        isMoving = false;
    }
    velocity = mouv * speed;
    Vector2f newPositionX = { position.x + velocity.x * deltaTime, position.y };
    Vector2f newPositionY = { position.x, position.y + velocity.y * deltaTime };
    if (map.isWalkable(newPositionX, playerSize)) {
        position.x = newPositionX.x;
    }
    if (map.isWalkable(newPositionY, playerSize)) {
        position.y = newPositionY.y;
    }
    shape.setPosition(position);
}

void Player::playerAttack() {
    if (Keyboard::isKeyPressed(Keyboard::J) && !isAttacking) {
        isAttacking = true;
        currentFrame = 0;
        speed = 100;
        setTexture(playerAttack1, 64, 32, 4, 0.1f);
        int index = rand() % 2;
        swordSwing[index].play();
    }
}

void Player::playFootstep() {
    if (footstepClock.getElapsedTime().asSeconds() > 0.4f) {
        int index = rand() % 6;
        footsteps[index].play();
        footstepClock.restart();
    }
}

void Player::dash() {
    if (Keyboard::isKeyPressed(Keyboard::LShift) && !isDashing && canDash) {
        int index = rand() % 2;
        Dash[index].play();
        isDashing = true;
        canDash = false;
        dashClock.restart();
        dashCooldownClock.restart();
        speed *= 2.5f;
    }

    if (isDashing) {
        setTexture(playerDash, 32, 32, 8, 0.1f);
        if (dashClock.getElapsedTime().asSeconds() > 0.2f) {
            isDashing = false;
            speed /= 2.5f;
        }
    }

    if (!canDash && dashCooldownClock.getElapsedTime().asSeconds() > 2.0f) {
        canDash = true;
    }
}

void Player::playerDie() {
    if (!isDying) {
        isDying = true;
        isMoving = false;
        isAttacking = false;
        speed = 0;
        currentFrame = 0;
    }

    if (currentFrame < 13) {
        setTexture(playerDeath, 32, 32, 14, 0.1f);
        if (cameraView.getSize().x > 200.f) {
            cameraView.zoom(0.75f);
        }
    }
    else {
        cout << "Le joueur est mort définitivement." << endl;
        playerDead = true;
        isDying = false; 
    }
}


void Player::playerWalk() {
    if (isDying || isAttacking) return;

    if (isMoving && !isDashing) {
        speed = 150.f;
        setTexture(playerRun, 32, 32, 8, 0.1f);
        playFootstep();
    }
    else if (!isDashing) {
        setTexture(playerIdle, 32, 32, 8, 0.1f);
    }
}

void Player::handleDeath() {
    if (isDying) {
        playerDie();
    }

    if (playerDead) {
        return;
    }
}

void Player::update(float deltaTime, const RenderWindow& window, const Vector2f& playerPosition, Map& map) {
    handleDeath();
    if (playerDead) return;
    dash();
    handleInput(deltaTime, map);
    playerAttack();
    playerWalk();
    hitbox.setPosition(position.x -9, position.y +4);
    hitboxV.x = position.x - 9;
    hitboxV.y = position.y + 4;

    animate(deltaTime);

    shape.setPosition(position);
    cameraView.setCenter(position);
    const_cast<RenderWindow&>(window).setView(cameraView);
}


void Player::draw(RenderWindow& window) {
    window.draw(shape);
    window.draw(hitbox);
}
void Player::reduceHealth(int damage) {
    health -= damage;
    cout << "Vie restante : " << health << endl;

    if (health <= 0 && !isDying) {
        cout << "Le joueur commence à mourir !" << endl;
        playerDie();
    }
}


void Player::setPosition(const Vector2f& newPosition) {
    position = newPosition;
    shape.setPosition(position);
}

void Player::setSpeed(float newSpeed) {
    speed = newSpeed;
}

Vector2f Player::getPosition() const {
    return hitboxV;
}

float Player::getSpeed() const {
    return speed;
}

float Player::getHealth() const {
    return health;
}

View Player::getCameraView() const {
    return cameraView;
}
