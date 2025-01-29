#include "../include/Player.h"
#include <iostream>
#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;

Player::Player() : speed(50.0f), position(100.0f, 100.0f), health(100), currentFrame(0), elapsedTime(0.0f) {
    playerShape.setSize({ 32, 32 });
    playerShape.setPosition(position);

    setTexture("assets/player/player_anim.png", 16, 19, 4, 0.1f);

}

void Player::handleInput() {
    Vector2f mouv(0.0f, 0.0f);

    if (Keyboard::isKeyPressed(Keyboard::Q)) {
        mouv.x -= 1.0f;
    }
    if (Keyboard::isKeyPressed(Keyboard::D)) {
        mouv.x += 1.0f;
    }
    if (Keyboard::isKeyPressed(Keyboard::Z)) {
        mouv.y -= 1.0f;
    }
    if (Keyboard::isKeyPressed(Keyboard::S)) {
        mouv.y += 1.0f;
    }

    float hypo = sqrt(mouv.x * mouv.x + mouv.y * mouv.y);
    if (hypo > 0.0f) {
        mouv.x /= hypo;
        mouv.y /= hypo;
    }

    position.x += mouv.x * speed * 0.1f;
    position.y += mouv.y * speed * 0.1f;
}

void Player::update(float deltaTime, const RenderWindow& window, const Vector2f& playerPosition) {
    handleInput();

    animate(deltaTime);

    Vector2u windowSize = window.getSize();
    float maxX = static_cast<float>(windowSize.x) - playerShape.getSize().x;
    float maxY = static_cast<float>(windowSize.y) - playerShape.getSize().y;

    if (position.x < 0) position.x = 0;
    if (position.y < 0) position.y = 0;
    if (position.x > maxX) position.x = maxX;
    if (position.y > maxY) position.y = maxY;

    playerShape.setPosition(position);
}

void Player::reduceHealth(int damage) {
    health -= damage;
    cout << health << endl;
    if (health <= 0) {
        cout << "Player is dead!" << endl;
    }
}

void Player::setPosition(const Vector2f& newPosition) {
    position = newPosition;
    playerShape.setPosition(position);
}

void Player::setSpeed(float newSpeed) {
    speed = newSpeed;
}

const RectangleShape& Player::getShape() const {
    return playerShape;
}

float Player::getSpeed() const {
    return speed;
}

float Player::getHealth() const {
    return health;
}

Vector2f Player::getPosition() const {
    return position;
}

void Player::draw(RenderWindow& window) {
    window.draw(playerShape);
}
