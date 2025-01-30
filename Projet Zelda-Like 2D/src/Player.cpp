#include "../include/Player.h"
#include <iostream>

using namespace sf;
using namespace std;

Player::Player() : speed(150.0f), position(100.0f, 100.0f), health(100) {
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

    if (!playerAttack1.loadFromFile("assets/player/player_attack1.png")) {
        cerr << "Texture player_attack1 not loaded!" << endl;
    }
    else {
        cout << "Texture player_attack1 loaded" << endl;
    }

    setTexture(playerIdle, 32, 32, 8, 0.1f);
    shape.setSize(Vector2f(32, 32));
    shape.setPosition(position);
    shape.setOrigin(shape.getSize().x/2, shape.getSize().y / 2);


    cameraView.setSize(400, 300);
    cameraView.setCenter(position);
}

void Player::handleInput(float deltaTime) {
    Vector2f mouv(0.0f, 0.0f);

    if (Keyboard::isKeyPressed(Keyboard::Q) || Keyboard::isKeyPressed(Keyboard::A)) mouv.x -= 1.0f, shape.setScale(-1, 1);;
    if (Keyboard::isKeyPressed(Keyboard::D)) mouv.x += 1.0f, shape.setScale(1, 1);
    if (Keyboard::isKeyPressed(Keyboard::Z) || Keyboard::isKeyPressed(Keyboard::W)) mouv.y -= 1.0f;
    if (Keyboard::isKeyPressed(Keyboard::S)) mouv.y += 1.0f;

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
    position += velocity * deltaTime;
}

void Player::playerAttack() {
    if (Keyboard::isKeyPressed(Keyboard::J) && !isAttacking) {
        attack();
        setTexture(playerAttack1, 64, 32, 4, 0.1f);
    }
}


void Player::playerWalk() {
    if (isAttacking) return;

    if (isMoving) {
        setTexture(playerRun, 32, 32, 8, 0.1f);
    }
    else {
        setTexture(playerIdle, 32, 32, 8, 0.1f);
    }
}


void Player::update(float deltaTime, const RenderWindow& window, const Vector2f& playerPosition) {
    handleInput(deltaTime);
    animate(deltaTime);
    if (!isAttacking) {
        playerWalk();
    }
    playerAttack();

    shape.setPosition(position);

    cameraView.setCenter(position);
    const_cast<RenderWindow&>(window).setView(cameraView);
}

void Player::draw(RenderWindow& window) {
    window.draw(shape);
}

void Player::reduceHealth(int damage) {
    health -= damage;
    cout << "Vie restante : " << health << endl;
    if (health <= 0) {
        cout << "Le joueur est mort !" << endl;
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
    return position;
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
