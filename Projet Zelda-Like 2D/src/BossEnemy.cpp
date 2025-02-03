#include "../include/BossEnemy.h"
#include <cmath>
#include <iostream>

BossEnemy::BossEnemy(float x, float y, float _detectionRange, Player& _player)
    : Enemy(x, y, 0.0f), detectionRange(_detectionRange), player(_player),
    isAttacking(false), playerEnteredArena(false), hasDoneIntroAttack(false),
    dialogue(320, 50), isSpawning(false), isDying(false) {

    attackCooldown = 5.0f;  // Idle entre les attaques
    currentPhase = BossPhase::INTRO_DIALOGUE;  // Le boss commence par parler

    dialogue.setPosition(x, y);
    dialogue.setDialogue({
        "Ahah ! Enfin un challenger digne de moi !",
        "Tu crois pouvoir me vaincre ?",
        "Prepare-toi a affronter ma puissance !"
        });

    loadTextures();
    shape.setTexture(nullptr);  // Début invisible
    shape.setPosition(x, y);
    shape.setOrigin(shape.getSize().x / 2, shape.getSize().y / 2);
}

/* ====================
    CHARGEMENT DES TEXTURES
   ==================== */
void BossEnemy::loadTextures() {
    if (!spawnTexture.loadFromFile("assets/enemy/boss/spawn.png"))
        std::cerr << "Erreur chargement SPAWN !" << std::endl;
    if (!deathTexture.loadFromFile("assets/enemy/boss/death.png"))
        std::cerr << "Erreur chargement DEATH !" << std::endl;
    if (!idleTexture.loadFromFile("assets/enemy/boss/idle.png"))
        std::cerr << "Erreur chargement IDLE !" << std::endl;
    if (!fireballTexture.loadFromFile("assets/enemy/boss/attack1.png"))
        std::cerr << "Erreur chargement FIREBALL !" << std::endl;
    if (!laserTexture.loadFromFile("assets/enemy/boss/attack2.png"))
        std::cerr << "Erreur chargement LASER !" << std::endl;
    if (!summonTexture.loadFromFile("assets/enemy/boss/attack3.png"))
        std::cerr << "Erreur chargement SUMMON !" << std::endl;
    if (!meteorTexture.loadFromFile("assets/enemy/boss/attack4.png"))
        std::cerr << "Erreur chargement METEOR !" << std::endl;
    if (!chargeTexture.loadFromFile("assets/enemy/boss/attack5.png"))
        std::cerr << "Erreur chargement CHARGE !" << std::endl;
}

/* ====================
    MISE À JOUR DU BOSS
   ==================== */
void BossEnemy::update(float deltaTime, const sf::RenderWindow& window, const sf::Vector2f& playerPosition, Map& map) {
    if (!playerEnteredArena) {
        checkPlayerEntry();
        return;
    }

    switch (currentPhase) {
    case BossPhase::INTRO_DIALOGUE: handleIntroDialogue(); break;
    case BossPhase::SPAWN: handleSpawnAnimation(); break;
    case BossPhase::IDLE: handleIdlePhase(); break;
    case BossPhase::ATTACKING: handleAttackingPhase(deltaTime); break;
    case BossPhase::DEAD: handleDeathPhase(); return;
    }

    animate(deltaTime);
}

/* ====================
    PHASES DU COMBAT
   ==================== */
void BossEnemy::checkPlayerEntry() {
    float distance = std::sqrt(std::pow(player.getPosition().x - getPosition().x, 2) +
        std::pow(player.getPosition().y - getPosition().y, 2));
    if (distance < 200.0f) {
        playerEnteredArena = true;
        phaseClock.restart();
        std::cout << "⚔️ Début du combat !" << std::endl;
    }
}

void BossEnemy::handleIntroDialogue() {
    if (!dialogue.isCurrentlyTyping() && !waitingForNextDialogue) {
        dialogueClock.restart();  // Démarre le délai d'attente
        waitingForNextDialogue = true;
    }

    if (waitingForNextDialogue && dialogueClock.getElapsedTime().asSeconds() > 1.0f) {
        dialogue.advanceDialogue();
        waitingForNextDialogue = false;
    }

    // Si le dialogue est terminé, passer à la phase de spawn
    if (dialogue.isDialogueFinished()) {
        currentPhase = BossPhase::SPAWN;
        startSpawnAnimation();
        phaseClock.restart();
    }

    dialogue.update();
}

void BossEnemy::handleSpawnAnimation() {
    if (phaseClock.getElapsedTime().asSeconds() > 3.0f) {
        currentPhase = BossPhase::IDLE;
        setTexture(idleTexture, 320, 320, 15, 0.1f);
        phaseClock.restart();
    }
}

void BossEnemy::handleIdlePhase() {
    if (phaseClock.getElapsedTime().asSeconds() > attackCooldown) {
        changePattern();
        currentPhase = BossPhase::ATTACKING;
        phaseClock.restart();
    }
}

void BossEnemy::handleAttackingPhase(float deltaTime) {
    executePattern(deltaTime);
    if (phaseClock.getElapsedTime().asSeconds() > attackDuration) {
        currentPhase = BossPhase::IDLE;
        setTexture(idleTexture, 320, 320, 15, 0.1f);
        phaseClock.restart();
    }
}

void BossEnemy::handleDeathPhase() {
    if (phaseClock.getElapsedTime().asSeconds() > 6.5f) {
        std::cout << "💀 Le Boss est mort définitivement !" << std::endl;
    }
}

/* ====================
    ANIMATIONS
   ==================== */
void BossEnemy::startSpawnAnimation() {
    std::cout << "🎬 Animation de spawn du boss commence !" << std::endl;
    currentPhase = BossPhase::SPAWN;
    shape.setTexture(&spawnTexture);  // Applique bien la texture !
    setTexture(spawnTexture, 320, 320, 30, 0.1f);
    phaseClock.restart();
    isSpawning = true;
}


void BossEnemy::startDeathAnimation() {
    currentPhase = BossPhase::DEAD;
    setTexture(deathTexture, 320, 320, 65, 0.1f);
    phaseClock.restart();
    isDying = true;
}

/* ====================
    GESTION DES ATTAQUES
   ==================== */
void BossEnemy::changePattern() {
    int randPattern = rand() % 5;
    switch (randPattern) {
    case 0: currentPattern = BossPattern::FIREBALLS; setTexture(fireballTexture, 320, 320, 17, 0.1f); attackDuration = 1.7f; break;
    case 1: currentPattern = BossPattern::LASER; setTexture(laserTexture, 320, 320, 24, 0.1f); attackDuration = 2.4f; break;
    case 2: currentPattern = BossPattern::SUMMON; setTexture(summonTexture, 320, 320, 70, 0.1f); attackDuration = 7.f; break;
    case 3: currentPattern = BossPattern::METEOR; setTexture(meteorTexture, 320, 320, 42, 0.1f); attackDuration = 4.2f; break;
    case 4: currentPattern = BossPattern::CHARGE; setTexture(chargeTexture, 320, 320, 42, 0.1f); attackDuration = 4.2f; break;
    }

    isAttacking = true;
    attackClock.restart();
}

void BossEnemy::executePattern(float deltaTime) {
    if (!isAttacking) return;

    switch (currentPattern) {
    case BossPattern::FIREBALLS: std::cout << "🔥 Le Boss tire des boules de feu !" << std::endl; break;
    case BossPattern::LASER:
        std::cout << "🔫 Le Boss tire un laser !" << std::endl;
        if (shape.getGlobalBounds().intersects(player.getShape().getGlobalBounds())) {
            player.reduceHealth(15);
        }
        break;
    case BossPattern::SUMMON: std::cout << "👹 Le Boss invoque des ennemis !" << std::endl; break;
    case BossPattern::METEOR: std::cout << "☄️ Le Boss fait tomber des météores !" << std::endl; break;
    case BossPattern::CHARGE: std::cout << "⚡ Le Boss charge vers le joueur !" << std::endl; break;
    }
}

void BossEnemy::draw(sf::RenderWindow& window) {
    window.draw(shape);
    dialogue.draw(window);  // Affichage du dialogue
}

