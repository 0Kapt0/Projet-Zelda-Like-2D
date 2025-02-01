#include "../include/Enemy.h"

using namespace sf;
using namespace std;

Enemy::Enemy(float x, float y, float _speed)
    : position(x, y), speed(_speed) {

    shape.setPosition(position);
}

void Enemy::update(float deltaTime, const RenderWindow& window, const Vector2f& playerPosition, Map& map) {
    animate(deltaTime);
    position.x += speed * deltaTime;

    Vector2u windowSize = window.getSize();
    if (position.x < 0) position.x = 0;
    if (position.x > windowSize.x - shape.getSize().x) position.x = windowSize.x - shape.getSize().x;

    shape.setPosition(position);
}


void Enemy::setPosition(const Vector2f& newPosition) {
    position = newPosition;
    shape.setPosition(position);
}

const RectangleShape& Enemy::getShape() const {
    return shape;
}

Vector2f Enemy::getPosition() const {
    return position;
}

void Enemy::draw(RenderWindow& window) {
    window.draw(shape);
}
