#include "../include/BossEnemy.h"
#include <cmath>
#include <iostream>

BossEnemy::BossEnemy(float x, float y, float _detectionRange, Player& _player)
    : Enemy(x, y, 0.0f), detectionRange(_detectionRange), player(_player),
    isAttacking(false), playerEnteredArena(false), hasDoneIntroAttack(false),
    dialogue(130, 30), isSpawning(false), isDying(false) {

    attackCooldown = 5.0f;
    currentPhase = BossPhase::INTRO_DIALOGUE;

    dialogue.setTextSound("assets/enemy/boss/bossText.wav");
    dialogue.setPosition(x - 80, y + 100);
    dialogue.setDialogue({
        "Ahah ! Enfin un challenger digne de moi !"
        "Depuis des siecles, j'attendais ce moment."
        "Tu penses pouvoir survivre ici ?",

        "Ces ruines etaient un bastion de lumiere,"
        "balayees par le Neant. Il ne reste que des ombres.",

        "Le Void est eternel, il efface toute chose."
        "Tu n'es qu'une lueur ephemere face a la nuit sans fin.",

        "Viens, mortel. Affronte l'inevitable !"
        });




    loadTextures();
    shape.setTexture(nullptr);
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

    if (!bossMusic.openFromFile("assets/enemy/boss/bossTheme.ogg")) {
        std::cerr << "Erreur chargement musique du boss !" << std::endl;
    }
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

    if (!isDying) {
        animate(deltaTime);
    }

    shakeDialogue();
}

/* ====================
    PHASES DU COMBAT
   ==================== */
void BossEnemy::checkPlayerEntry() {
    float distance = std::sqrt(std::pow(player.getPosition().x - getPosition().x, 2) +
        std::pow(player.getPosition().y - getPosition().y, 2));
    if (distance < 250.0f) {
        playerEnteredArena = true;
        phaseClock.restart();
        std::cout << "Début du combat !" << std::endl;
    }
}

void BossEnemy::handleIntroDialogue() {
    if (!dialogue.isCurrentlyTyping() && !waitingForNextDialogue) {
        dialogueClock.restart();
        waitingForNextDialogue = true;
    }

    if (waitingForNextDialogue && dialogueClock.getElapsedTime().asSeconds() > 1.0f) {
        dialogue.advanceDialogue();
        waitingForNextDialogue = false;

        // Vérifie si on est sur la dernière ligne du dialogue
        if (dialogue.getCurrentDialogueIndex() == dialogue.getDialogueSize() - 1) {
            isShaking = true;
            shakeClock.restart();
        }
    }

    dialogue.update();

    if (dialogue.isDialogueFinished()) {
        std::cout << "Dialogue terminé, début du spawn !" << std::endl;
        currentPhase = BossPhase::SPAWN;
        isSpawning = false;
        startSpawnAnimation();
        phaseClock.restart();
    }
}


void BossEnemy::shakeDialogue() {
    if (isShaking && shakeClock.getElapsedTime().asSeconds() < shakeDuration) {
        float offsetX = (rand() % 6 - 3);
        float offsetY = (rand() % 6 - 3);
        dialogue.setPosition((shape.getPosition().x - 80) + offsetX, (shape.getPosition().y + 100) + offsetY);
    }
    else {
        isShaking = false;
        dialogue.setPosition(shape.getPosition().x - 80, shape.getPosition().y + 100);
    }
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
    if (!isAttacking) return;

    executePattern(deltaTime);

    if (currentFrame >= totalFrames - 1) {
        std::cout << "Animation d'attaque terminee, passage en IDLE." << std::endl;
        currentPhase = BossPhase::IDLE;
        setTexture(idleTexture, 320, 320, 15, 0.1f);
        isAttacking = false;
        phaseClock.restart();
    }
}



void BossEnemy::handleDeathPhase() {
    if (phaseClock.getElapsedTime().asSeconds() > 6.5f) {
        std::cout << "Le Boss est mort definitivement !" << std::endl;
    }
}

/* ====================
    ANIMATIONS
   ==================== */
void BossEnemy::startSpawnAnimation() {
    if (!isSpawning) {
        std::cout << "Lancement de la musique du Boss !" << std::endl;
        bossMusic.setLoop(true);
        bossMusic.setVolume(10);
        bossMusic.play();

        std::cout << "Animation de spawn commence !" << std::endl;
        currentPhase = BossPhase::SPAWN;
        currentFrame = 0;
        shape.setTexture(&spawnTexture);
        setTexture(spawnTexture, 320, 320, 30, 0.1f);
        phaseClock.restart();
        isSpawning = true;
    }
}



void BossEnemy::startDeathAnimation() {
    currentPhase = BossPhase::DEAD;
    currentFrame = 0;
    setTexture(deathTexture, 320, 320, 65, 0.1f);
    phaseClock.restart();
    isDying = true;

    std::cout << "Boss mort, arrêt de la musique !" << std::endl;
    bossMusic.stop();
}


/* ====================
    GESTION DES ATTAQUES
   ==================== */
void BossEnemy::changePattern() {
    if (isAttacking) return;

    int randPattern = rand() % 5;
    currentFrame = 0;

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
    case BossPattern::FIREBALLS: std::cout << "Le Boss tire des boules de feu !" << std::endl; break;
    case BossPattern::LASER:std::cout << "Le Boss tire un laser !" << std::endl; break;
    case BossPattern::SUMMON: std::cout << "Le Boss invoque des ennemis !" << std::endl; break;
    case BossPattern::METEOR: std::cout << "Le Boss fait tomber des meteores !" << std::endl; break;
    case BossPattern::CHARGE: std::cout << "Le Boss charge vers le joueur !" << std::endl; break;
    }

    if (attackClock.getElapsedTime().asSeconds() > attackDuration) {
        std::cout << "Fin de l'attaque, retour en IDLE." << std::endl;
        isAttacking = false;
        currentPhase = BossPhase::IDLE;
        setTexture(idleTexture, 320, 320, 15, 0.1f);
        phaseClock.restart();
    }
}

void BossEnemy::draw(sf::RenderWindow& window) {
    window.draw(shape);
    dialogue.draw(window);
}


