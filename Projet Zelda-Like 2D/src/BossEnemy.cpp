#include "../include/BossEnemy.h"
#include <cmath>
#include <iostream>

BossEnemy::BossEnemy(float x, float y, float _detectionRange, Player& _player)
    : Enemy(x, y, 0.0f), detectionRange(_detectionRange), player(_player), isAttacking(false) {

    attackCooldown = 3.0f; // Le boss change de pattern toutes les 3 secondes
    currentPattern = BossPattern::IDLE;

    if (!idleTexture.loadFromFile("assets/boss/boss_idle.png")) {
        std::cerr << "Erreur de chargement de la texture IDLE du Boss !" << std::endl;
    }
    if (!fireballTexture.loadFromFile("assets/boss/boss_fireball.png")) {
        std::cerr << "Erreur de chargement de la texture FIREBALL du Boss !" << std::endl;
    }
    if (!laserTexture.loadFromFile("assets/boss/boss_laser.png")) {
        std::cerr << "Erreur de chargement de la texture LASER du Boss !" << std::endl;
    }
    if (!summonTexture.loadFromFile("assets/boss/boss_summon.png")) {
        std::cerr << "Erreur de chargement de la texture SUMMON du Boss !" << std::endl;
    }

    shape.setTexture(&idleTexture);
    setTexture(idleTexture, 100, 100, 4, 0.2f); // Animation de base

    setPosition(sf::Vector2f(x, y));
    shape.setOrigin(shape.getSize().x / 2, shape.getSize().y / 2);
}

void BossEnemy::update(float deltaTime, const sf::RenderWindow& window, const sf::Vector2f& playerPosition, Map& map) {
    float distance = std::sqrt(std::pow(playerPosition.x - getPosition().x, 2) + std::pow(playerPosition.y - getPosition().y, 2));

    // Vérifie si le joueur est dans la portée d'attaque
    if (distance < detectionRange) {
        if (attackClock.getElapsedTime().asSeconds() > attackCooldown) {
            changePattern();
            attackClock.restart();
        }
        executePattern(deltaTime);
    }

    animate(deltaTime);
}

void BossEnemy::changePattern() {
    int randPattern = rand() % 3; // Choisit un pattern aléatoire (hors IDLE)
    switch (randPattern) {
    case 0:
        currentPattern = BossPattern::FIREBALLS;
        shape.setTexture(&fireballTexture);
        setTexture(fireballTexture, 100, 100, 4, 0.2f);
        break;
    case 1:
        currentPattern = BossPattern::LASER;
        shape.setTexture(&laserTexture);
        setTexture(laserTexture, 100, 100, 4, 0.2f);
        break;
    case 2:
        currentPattern = BossPattern::SUMMON;
        shape.setTexture(&summonTexture);
        setTexture(summonTexture, 100, 100, 4, 0.2f);
        break;
    }
    isAttacking = true;
}

void BossEnemy::executePattern(float deltaTime) {
    if (!isAttacking) return;

    switch (currentPattern) {
    case BossPattern::FIREBALLS:
        std::cout << "Le Boss tire des boules de feu !" << std::endl;
        // TODO: Ajouter des projectiles
        break;
    case BossPattern::LASER:
        std::cout << "Le Boss tire un laser !" << std::endl;
        if (shape.getGlobalBounds().intersects(player.getShape().getGlobalBounds())) {
            player.reduceHealth(15);
        }
        break;
    case BossPattern::SUMMON:
        std::cout << "Le Boss invoque des ennemis !" << std::endl;
        // TODO: Ajouter des ennemis invoqués
        break;
    default:
        break;
    }

    // Après l'attaque, repasser en IDLE
    if (attackClock.getElapsedTime().asSeconds() > 2.0f) {
        currentPattern = BossPattern::IDLE;
        shape.setTexture(&idleTexture);
        setTexture(idleTexture, 100, 100, 4, 0.2f);
        isAttacking = false;
    }
}

void BossEnemy::draw(sf::RenderWindow& window) {
    window.draw(shape);
}
