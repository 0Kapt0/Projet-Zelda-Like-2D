#include "../include/ChaserEnemy.h"
#include <cmath>
#include <iostream>

ChaserEnemy::ChaserEnemy(float x, float y, float _speed, float _detectionRange, Player& _player)
    : Enemy(x, y, _speed), detectionRange(_detectionRange), player(_player), isAttacking(false) {


    speed = _speed;
    attackCooldownTime = 1.0f;

    if (!texture.loadFromFile("assets/enemy/necro/necro_anim.png")) {
        std::cerr << "Erreur de chargement de la texture du ChaserEnemy !" << std::endl;
    }
    if (!attack.loadFromFile("assets/enemy/necro/necro_attack.png")) {
        std::cerr << "Erreur de chargement de la texture d'attaque du ChaserEnemy !" << std::endl;
    }

    shape.setTexture(&texture);
    setTexture(texture, 48, 50, 8, 0.1f);

    setPosition(Vector2f(x, y));
    shape.setOrigin(shape.getSize().x / 2, shape.getSize().y / 2);
}

void ChaserEnemy::update(float deltaTime, const RenderWindow& window, const Vector2f& playerPosition, Map& map) {
    Vector2f direction = playerPosition - getPosition();
    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    if (distance > 0.0f) {
        direction /= distance;
    }

    // Tourne l'ennemi selon la direction X
    if (direction.x > 0) {
        shape.setScale(1, 1);
    }
    else if (direction.x < 0) {
        shape.setScale(-1, 1);
    }

    // Gestion de l'attaque
    if (isAttacking) {
        animate(deltaTime);

        if (attackCooldown.getElapsedTime().asSeconds() > 0.72f) {
            isAttacking = false;
            speed = 50;
            currentFrame = 0;
            elapsedTime = 0.0f;
            setTexture(texture, 48, 50, 8, 0.1f);
        }
    }

    if (!isAttacking && distance < detectionRange) {
        Vector2f newPosition = getPosition() + (direction * speed * deltaTime);

        if (map.isWalkable(newPosition, shape.getSize(), shape.getGlobalBounds())) {
            position = newPosition;
            shape.setPosition(position);
        }
    }

    if (!player.getIsDashing())
        if (shape.getGlobalBounds().intersects(player.getShape().getGlobalBounds())) {
            if (attackCooldown.getElapsedTime().asSeconds() > attackCooldownTime) {
                isAttacking = true;
                attackCooldown.restart();
                player.reduceHealth(10);

                speed /= 2.0f;

                currentFrame = 0;
                elapsedTime = 0.0f;
                setTexture(attack, 80, 65, 8, 0.15f);
            }
        }
    animate(deltaTime);
}

void ChaserEnemy::draw(RenderWindow& window) {
    window.draw(shape);
}
