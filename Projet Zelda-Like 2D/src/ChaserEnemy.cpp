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

        // Vérifier si l'animation d'attaque est terminée
        if (attackCooldown.getElapsedTime().asSeconds() > 0.72f) {
            isAttacking = false;
            speed = 50;
            currentFrame = 0;
            elapsedTime = 0.0f;

            // **Appliquer les dégâts à la fin de l'animation**
            player.reduceHealth(10);

            setTexture(texture, 48, 50, 8, 0.1f); // Remettre l'animation de base
        }
        return; // Ne pas faire d'autres mises à jour tant que l'attaque est en cours
    }

    // Déplacement si l'ennemi n'est pas en train d'attaquer
    if (distance < detectionRange) {
        Vector2f newPosition = getPosition() + (direction * speed * deltaTime);

        if (map.isWalkable(newPosition, shape.getSize(), shape.getGlobalBounds())) {
            position = newPosition;
            shape.setPosition(position);
        }
    }

    // Début de l'attaque si le joueur est touché et pas en dash
    if (!player.getIsDashing() && shape.getGlobalBounds().intersects(player.getShape().getGlobalBounds())) {
        if (attackCooldown.getElapsedTime().asSeconds() > attackCooldownTime) {
            isAttacking = true;
            attackCooldown.restart();

            speed /= 2.0f; // Ralentir l'ennemi pendant l'attaque

            currentFrame = 0;
            elapsedTime = 0.0f;
            setTexture(attack, 80, 65, 8, 0.15f); // Changer l'animation pour celle d'attaque
        }
    }

    animate(deltaTime);
}

void ChaserEnemy::draw(RenderWindow& window) {
    window.draw(shape);
}
