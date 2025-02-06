#include "../include/ChaserEnemy.h"
#include <cmath>
#include <iostream>

ChaserEnemy::ChaserEnemy(float x, float y, float _speed, float _health, float _detectionRange, Player& _player)
    : Enemy(x, y, _speed, _health), detectionRange(_detectionRange), player(_player), isAttacking(false) {

    originalSpeed = _speed;
    speedDuringAttack = _speed / 2;
    attackCooldownTime = 1.0f;

    if (!texture.loadFromFile("assets/enemy/necro/necro_anim.png")) {
        cerr << "Erreur de chargement de la texture du ChaserEnemy !" << endl;
    }
    if (!attack.loadFromFile("assets/enemy/necro/necro_attack.png")) {
        cerr << "Erreur de chargement de la texture d'attaque du ChaserEnemy !" << endl;
    }
    if (!death.loadFromFile("assets/enemy/necro/necro_death.png")) {
        cerr << "Erreur de chargement de la texture de mort du ChaserEnemy !" << endl;
    }

    shape.setTexture(&texture);
    setTexture(texture, 48, 65, 8, 0.1f);

    setPosition(Vector2f(x, y));
}

//Vérifie si l'ennemi est touché par l'attaque du joueur
void ChaserEnemy::checkPlayerAttack() {
    if (damageCooldown.getElapsedTime().asSeconds() < damageCooldownTime) return;

    if (shape.getGlobalBounds().intersects(player.getAttackHitbox()) && player.playerAttacking()) {
        reduceHealth(20);
        cout << "L'ennemi a ete touche par l'attaque du joueur !" << endl;
        damageCooldown.restart();
    }

    if (health <= 0 && !isDying) {
        isDying = true;
        speed = 0;

        if (!deathAnimationFinished) {
            setTexture(death, 48, 48, 11, 1.0f);
            currentFrame = 0;
            elapsedTime = 0;
            deathAnimationClock.restart();
        }
    }
}

void ChaserEnemy::handleDeath() {
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



void ChaserEnemy::update(float deltaTime, const RenderWindow& window, const Vector2f& playerPosition, Map& map) {
    if (isDying) {
        updateHealthBar();
        handleDeath();
        return;
    }

    updateHealthBar();

    checkPlayerAttack();

    Vector2f direction = playerPosition - getPosition();
    float distance = sqrt(direction.x * direction.x + direction.y * direction.y);

    if (distance > 0.0f) {
        direction /= distance;
    }

    //Tourne l'ennemi vers le joueur
    if (direction.x > 0) {
        shape.setScale(1, 1);
    }
    else if (direction.x < 0) {
        shape.setScale(-1, 1);
    }

    float currentSpeed = isAttacking ? speedDuringAttack : originalSpeed;

    if (distance < detectionRange) {
        Vector2f newPosition = getPosition() + (direction * currentSpeed * deltaTime);

        position = newPosition;
        shape.setPosition(position);

        /*if (map.isWalkable(newPosition, shape.getSize(), shape.getGlobalBounds())) {
            position = newPosition;
            shape.setPosition(position);
        }*/
    }

    //Gestion de l'attaque
    if (isAttacking && !isDying) {
        animate(deltaTime);

        if (currentFrame == 4 && !player.getIsDashing() && shape.getGlobalBounds().intersects(player.getShape().getGlobalBounds())) {
            player.reduceHealth(10);
        }

        if (attackCooldown.getElapsedTime().asSeconds() > 1.2f) {
            isAttacking = false;
            speed = originalSpeed;
            currentFrame = 0;
            elapsedTime = 0.0f;
            setTexture(texture, 48, 65, 8, 0.1f);
        }
        return;
    }

    if (!player.getIsDashing() && shape.getGlobalBounds().intersects(player.getShape().getGlobalBounds())) {
        if (attackCooldown.getElapsedTime().asSeconds() > attackCooldownTime) {
            isAttacking = true;
            attackCooldown.restart();
            speed = speedDuringAttack;
            currentFrame = 0;
            elapsedTime = 0.0f;
            setTexture(attack, 80, 65, 8, 0.15f);
        }
    }

    animate(deltaTime);
}

void ChaserEnemy::draw(RenderWindow& window) {
    if (isDead) return;
    window.draw(shape);
    window.draw(healthBarOutline);
    window.draw(healthBar);
}
