#include "../include/Player.h"
#include <iostream>

using namespace sf;
using namespace std;

Player::Player() : speed(150.0f), position(100.0f, 100.0f), health(100) {
    if (!playerTexture.loadFromFile("assets/player/player_anim.png")) {
        cerr << "Texture not loaded!" << endl;
    }
    else {
        cout << "Texture loaded" << endl;
    }

    setTexture(playerTexture, 16, 19, 4, 0.1f);
    shape.setSize(Vector2f(16, 19));
    shape.setPosition(position);


    cameraView.setSize(400, 300);
    cameraView.setCenter(position);
}

void Player::handleInput(float deltaTime) {
    Vector2f mouv(0.0f, 0.0f);

    if (Keyboard::isKeyPressed(Keyboard::Q) || Keyboard::isKeyPressed(Keyboard::A)) mouv.x -= 1.0f;
    if (Keyboard::isKeyPressed(Keyboard::D)) mouv.x += 1.0f;
    if (Keyboard::isKeyPressed(Keyboard::Z) || Keyboard::isKeyPressed(Keyboard::W)) mouv.y -= 1.0f;
    if (Keyboard::isKeyPressed(Keyboard::S)) mouv.y += 1.0f;

    if (mouv.x != 0 || mouv.y != 0) {
        float length = sqrt(mouv.x * mouv.x + mouv.y * mouv.y);
        mouv /= length;
    }

    velocity = mouv * speed;
    position += velocity * deltaTime;
}

void Player::update(float deltaTime, const RenderWindow& window, const Vector2f& playerPosition) {
    handleInput(deltaTime);
    animate(deltaTime);

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
