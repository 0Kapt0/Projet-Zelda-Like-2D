#include "../include/BossEnemy.h"
#include <cmath>
#include <iostream>

using namespace sf;
using namespace std;

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
        cerr << "Erreur chargement SPAWN !" << endl;
    if (!deathTexture.loadFromFile("assets/enemy/boss/death.png"))
        cerr << "Erreur chargement DEATH !" << endl;
    if (!idleTexture.loadFromFile("assets/enemy/boss/idle.png"))
        cerr << "Erreur chargement IDLE !" << endl;
    if (!fireballTexture.loadFromFile("assets/enemy/boss/attack1.png"))
        cerr << "Erreur chargement FIREBALL !" << endl;
    if (!laserTexture.loadFromFile("assets/enemy/boss/attack2.png"))
        cerr << "Erreur chargement LASER !" << endl;
    if (!summonTexture.loadFromFile("assets/enemy/boss/attack3.png"))
        cerr << "Erreur chargement SUMMON !" << endl;
    if (!meteorTexture.loadFromFile("assets/enemy/boss/attack4.png"))
        cerr << "Erreur chargement METEOR !" << endl;
    if (!chargeTexture.loadFromFile("assets/enemy/boss/attack5.png"))
        cerr << "Erreur chargement CHARGE !" << endl;

    if (!bossMusic.openFromFile("assets/enemy/boss/bossTheme.ogg")) {
        cerr << "Erreur chargement musique du boss !" << endl;
    }
}

/* ====================
    MISE À JOUR DU BOSS
   ==================== */
void BossEnemy::update(float deltaTime, const RenderWindow& window, const Vector2f& playerPosition, Map& map) {
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
    float distance = sqrt(pow(player.getPosition().x - getPosition().x, 2) +
        pow(player.getPosition().y - getPosition().y, 2));
    if (distance < 250.0f) {
        playerEnteredArena = true;
        phaseClock.restart();
        cout << "Début du combat !" << endl;
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
        cout << "Dialogue terminé, début du spawn !" << endl;
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
        cout << "Animation d'attaque terminee, passage en IDLE." << endl;
        currentPhase = BossPhase::IDLE;
        setTexture(idleTexture, 320, 320, 15, 0.1f);
        isAttacking = false;
        phaseClock.restart();
    }
}



void BossEnemy::handleDeathPhase() {
    if (phaseClock.getElapsedTime().asSeconds() > 6.5f) {
        cout << "Le Boss est mort definitivement !" << endl;
    }
}

/* ====================
    ANIMATIONS
   ==================== */
void BossEnemy::startSpawnAnimation() {
    if (!isSpawning) {
        cout << "Lancement de la musique du Boss !" << endl;
        bossMusic.setLoop(true);
        bossMusic.setVolume(10);
        bossMusic.play();

        cout << "Animation de spawn commence !" << endl;
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

    cout << "Boss mort, arrêt de la musique !" << endl;
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
    case BossPattern::FIREBALLS: cout << "Le Boss tire des boules de feu !" << endl; break;
    case BossPattern::LASER:cout << "Le Boss tire un laser !" << endl; break;
    case BossPattern::SUMMON: cout << "Le Boss invoque des ennemis !" << endl; break;
    case BossPattern::METEOR: cout << "Le Boss fait tomber des meteores !" << endl; break;
    case BossPattern::CHARGE: cout << "Le Boss charge vers le joueur !" << endl; break;
    }

    if (attackClock.getElapsedTime().asSeconds() > attackDuration) {
        cout << "Fin de l'attaque, retour en IDLE." << endl;
        isAttacking = false;
        currentPhase = BossPhase::IDLE;
        setTexture(idleTexture, 320, 320, 15, 0.1f);
        phaseClock.restart();
    }
}

void BossEnemy::draw(RenderWindow& window) {
    window.draw(shape);
    dialogue.draw(window);
}


