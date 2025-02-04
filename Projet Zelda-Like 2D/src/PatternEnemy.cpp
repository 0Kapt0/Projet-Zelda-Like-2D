#include "../include/PatternEnemy.h"
#include <cmath>
#include <iostream>

PatternEnemy::PatternEnemy(float x, float y, float _speed, float _health, Player& _player)
    : Enemy(x, y, _speed, _health), player(_player), isAttacking(false) {

    speed = _speed;

    if (!texture.loadFromFile("assets/enemy/slime/slime_walk.png")) {
        std::cerr << "Erreur de chargement de la texture du PatternEnemy !" << std::endl;
    }
    if (!death.loadFromFile("assets/enemy/slime/slime_death.png")) {
        std::cerr << "Erreur de chargement de la texture de mort du PatternEnemy !" << std::endl;
    }
    else {
        shape.setTexture(&texture);
        setTexture(texture, 32, 17, 7, 0.1f);
    }

    //Positionne l'ennemi au bon endroit
    setPosition(Vector2f(x, y));
    shape.setOrigin(shape.getSize().x / 2, shape.getSize().y / 2);

    direction.x = -10;
}

void PatternEnemy::checkPlayerAttack() {
    if (damageCooldown.getElapsedTime().asSeconds() < damageCooldownTime) return;

    if (shape.getGlobalBounds().intersects(player.getAttackHitbox()) && player.playerAttacking()) {
        reduceHealth(20);
        std::cout << "L'ennemi a ete touche par l'attaque du joueur !" << std::endl;
        damageCooldown.restart();
    }

    if (health <= 0 && !isDying) {
        isDying = true;
        speed = 0;

        if (!deathAnimationFinished) {
            setTexture(death, 32, 16, 16, 1.0f);
            currentFrame = 0;
            elapsedTime = 0;
            deathAnimationClock.restart();
        }
    }
}

void PatternEnemy::handleDeath() {
    if (!isDying) return;

    if (!deathAnimationFinished) {
        animate(0.1f);
    }

    if (currentFrame >= totalFrames - 1) {
        deathAnimationFinished = true;
        isDead = true;
        return;
    }
}

void PatternEnemy::update(float deltaTime, const RenderWindow& window, const Vector2f& playerPosition, Map& map) {
    if (isDying) {
        updateHealthBar();
        handleDeath();
        return;
    }

    updateHealthBar();

    checkPlayerAttack();

    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    Vector2f newPosition = getPosition() + (direction * speed * deltaTime);

    //Vérification de la collision
    if (map.isWalkable(newPosition, shape.getSize(), shape.getGlobalBounds())) {
        position = newPosition;
        shape.setPosition(position);
    }
    else {
        if (direction.x == -10) {
            shape.move(10, 0);
            direction.x = 10;
            shape.setScale(-1, 1);
        }
        else if (direction.x == 10) {
            shape.move(-10, 0);
            direction.x = -10;
            shape.setScale(1, 1);
        }
    }

    if (shape.getGlobalBounds().intersects(player.getShape().getGlobalBounds())) {
        player.reduceHealth(10);
    }

    animate(deltaTime);
}

void PatternEnemy::draw(RenderWindow& window) {
    if (isDead) return;
    window.draw(shape);
    window.draw(healthBarOutline);
    window.draw(healthBar);
}
