#include "../include/BossEnemy.h"
#include <cmath>
#include <iostream>

BossEnemy::BossEnemy(float x, float y, float _detectionRange, Player& _player)
    : Enemy(x, y, 0.0f), detectionRange(_detectionRange), player(_player), isAttacking(false), isSpawning(true), isDying(false) {

    attackCooldown = 3.0f;
    currentPattern = BossPattern::SPAWN;  // Commence par l'animation de spawn

    if (!spawnTexture.loadFromFile("assets/enemy/boss/spawn.png")) {
        std::cerr << "Erreur de chargement de la texture SPAWN du Boss !" << std::endl;
    }
    if (!deathTexture.loadFromFile("assets/enemy/boss/death.png")) {
        std::cerr << "Erreur de chargement de la texture DEATH du Boss !" << std::endl;
    }
    if (!idleTexture.loadFromFile("assets/enemy/boss/idle.png")) {
        std::cerr << "Erreur de chargement de la texture IDLE du Boss !" << std::endl;
    }
    if (!fireballTexture.loadFromFile("assets/enemy/boss/attack1.png")) {
        std::cerr << "Erreur de chargement de la texture FIREBALL du Boss !" << std::endl;
    }
    if (!laserTexture.loadFromFile("assets/enemy/boss/attack2.png")) {
        std::cerr << "Erreur de chargement de la texture LASER du Boss !" << std::endl;
    }
    if (!summonTexture.loadFromFile("assets/enemy/boss/attack3.png")) {
        std::cerr << "Erreur de chargement de la texture SUMMON du Boss !" << std::endl;
    }
    if (!meteorTexture.loadFromFile("assets/enemy/boss/attack4.png")) {
        std::cerr << "Erreur de chargement de la texture METEOR du Boss !" << std::endl;
    }
    if (!chargeTexture.loadFromFile("assets/enemy/boss/attack5.png")) {
        std::cerr << "Erreur de chargement de la texture CHARGE du Boss !" << std::endl;
    }

    shape.setTexture(&spawnTexture);
    setTexture(spawnTexture, 320, 320, 30, 0.1); // Animation de spawn

    setPosition(sf::Vector2f(x, y));
    shape.setOrigin(shape.getSize().x / 2, shape.getSize().y / 2);
}

void BossEnemy::startSpawnAnimation() {
    currentPattern = BossPattern::SPAWN;
    setTexture(spawnTexture, 320, 320, 30, 0.1f);  // 30 frames, 3.0s
    attackClock.restart();
    isSpawning = true;
}

void BossEnemy::update(float deltaTime, const sf::RenderWindow& window, const sf::Vector2f& playerPosition, Map& map) {
    if (currentPattern == BossPattern::SPAWN) {
        if (attackClock.getElapsedTime().asSeconds() > 3.0f) {
            currentPattern = BossPattern::IDLE;
            setTexture(idleTexture, 320, 320, 15, 0.1f);
            isSpawning = false;
        }
        animate(deltaTime);
        return;
    }

    if (Keyboard::isKeyPressed(Keyboard::X)) startDeathAnimation();

    float distance = std::sqrt(std::pow(playerPosition.x - getPosition().x, 2) + std::pow(playerPosition.y - getPosition().y, 2));

    if (distance < detectionRange) {
        if (attackClock.getElapsedTime().asSeconds() > attackCooldown) {
            changePattern();
            attackClock.restart();
        }
        executePattern(deltaTime);
    }

    if (currentPattern == BossPattern::DEAD) {
        if (attackClock.getElapsedTime().asSeconds() > 6.5f) {
            std::cout << "Le Boss est mort définitivement !" << std::endl;
            // Ici, on pourrait supprimer le boss ou déclencher un événement
        }
        animate(deltaTime);
        return;
    }

    animate(deltaTime);
}

void BossEnemy::startDeathAnimation() {
    currentPattern = BossPattern::DEAD;
    setTexture(deathTexture, 320, 320, 65, 0.1f);  // 65 frames, 6.5s
    attackClock.restart();
    isDying = true;
}


void BossEnemy::changePattern() {
    int randPattern = rand() % 5; // Sélection aléatoire parmi 5 attaques
    switch (randPattern) {
    case 0:
        currentPattern = BossPattern::FIREBALLS;
        setTexture(fireballTexture, 320, 320, 17, 0.1f);  // 17 frames, 1.7s
        attackDuration = 1.7f;
        break;
    case 1:
        currentPattern = BossPattern::LASER;
        setTexture(laserTexture, 320, 320, 24, 0.1f);  // 24 frames, 2.4s
        attackDuration = 2.4f;
        break;
    case 2:
        currentPattern = BossPattern::SUMMON;
        setTexture(summonTexture, 320, 320, 70, 0.1f);  // 70 frames, 7s
        attackDuration = 7.f;
        break;
    case 3:
        currentPattern = BossPattern::METEOR;
        setTexture(meteorTexture, 320, 320, 42, 0.1f);  // 42 frames, 4.2s
        attackDuration = 4.2f;
        break;
    case 4:
        currentPattern = BossPattern::CHARGE;
        setTexture(chargeTexture, 320, 320, 42, 0.1f);  // 42 frames, 4.2s
        attackDuration = 4.2f;
        break;
    }

    isAttacking = true;
    attackClock.restart();  // Redémarrer le temps pour mesurer la durée de l'attaque
}


void BossEnemy::executePattern(float deltaTime) {
    if (!isAttacking) return;

    switch (currentPattern) {
    case BossPattern::FIREBALLS:
        std::cout << "🔥 Le Boss tire des boules de feu !" << std::endl;
        break;
    case BossPattern::LASER:
        std::cout << "🔫 Le Boss tire un laser !" << std::endl;
        if (shape.getGlobalBounds().intersects(player.getShape().getGlobalBounds())) {
            player.reduceHealth(15);
        }
        break;
    case BossPattern::SUMMON:
        std::cout << "👹 Le Boss invoque des ennemis !" << std::endl;
        break;
    case BossPattern::METEOR:
        std::cout << "☄️ Le Boss fait tomber des météores !" << std::endl;
        break;
    case BossPattern::CHARGE:
        std::cout << "⚡ Le Boss charge vers le joueur !" << std::endl;
        break;
    }

    // Après `attackDuration` secondes, retour à IDLE
    if (attackClock.getElapsedTime().asSeconds() > attackDuration) {
        currentPattern = BossPattern::IDLE;
        setTexture(idleTexture, 320, 320, 15, 0.1f);
        isAttacking = false;
    }
}


void BossEnemy::draw(sf::RenderWindow& window) {
    window.draw(shape);
}
