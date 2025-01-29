#include "../include/Enemy.h"

Enemy::Enemy(float x, float y, float _speed)
    : position(x, y), speed(_speed), animationTimer(0.0f), frameDuration(0.1f), currentFrame(0), totalFrames(1) {
    shape.setPosition(position);
}

void Enemy::update(float deltaTime, const RenderWindow& window, const Vector2f& playerPosition) {
    animate(deltaTime);
    shape.setPosition(position);
}

const RectangleShape& Enemy::getShape() const {
    return shape;
}

Vector2f Enemy::getPosition() const {
    return position;
}

void Enemy::setPosition(const Vector2f& newPosition) {
    position = newPosition;
    shape.setPosition(position);
}


void Enemy::draw(RenderWindow& window) {
    window.draw(shape);
}
