#include "../include/Enemy.h"

Enemy::Enemy(float x, float y, float _speed, float _health)
    : position(x, y), speed(_speed), health(_health), maxHealth(_health) {
    shape.setPosition(position);
    initializeHealthBar();
}

void Enemy::update(float deltaTime, const RenderWindow& window, const Vector2f& playerPosition, Map& map) {
    animate(deltaTime);
}

const RectangleShape& Enemy::getShape() const {
    return shape;
}

void Enemy::initializeHealthBar() {
    healthBar.setSize(Vector2f(40.0f, 5.0f));
    healthBar.setFillColor(Color(100, 0, 0, 255));
    healthBar.setOrigin(healthBar.getSize().x / 2, healthBar.getSize().y / 2);

    healthBarOutline.setSize(Vector2f(40.0f, 5.0f));
    healthBarOutline.setFillColor(Color::Transparent);
    healthBarOutline.setOutlineThickness(0.5f);
    healthBarOutline.setOutlineColor(Color::Black);
    healthBarOutline.setOrigin(healthBarOutline.getSize().x / 2, healthBarOutline.getSize().y / 2);
}

void Enemy::updateHealthBar() {
    float healthPercentage = health / maxHealth;
    healthBar.setSize(Vector2f(40.0f * healthPercentage, 5.0f));

    Vector2f enemyPosition = shape.getPosition();
    healthBar.setPosition(enemyPosition.x, enemyPosition.y - 5);
    healthBarOutline.setPosition(enemyPosition.x, enemyPosition.y - 5);
}


void Enemy::reduceHealth(float damage) {
    if (health <= 0) return;

    health -= damage;
    if (health < 0) health = 0;

    std::cout << "L'ennemi a subi " << damage << " points de degats ! Vie restante : " << health << std::endl;

    if (health <= 0) {
        std::cout << "L'ennemi est mort !" << std::endl;
    }
}


void Enemy::setPosition(const Vector2f& newPosition) {
    position = newPosition;
    shape.setPosition(newPosition);
}

void Enemy::draw(RenderWindow& window) {
    window.draw(shape);
    window.draw(healthBarOutline);
    window.draw(healthBar);
}

