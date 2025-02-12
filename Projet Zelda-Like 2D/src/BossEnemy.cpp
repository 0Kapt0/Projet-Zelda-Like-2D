﻿#include "../include/BossEnemy.h"
#include <cmath>
#include <iostream>

using namespace sf;
using namespace std;

BossEnemy::BossEnemy(float x, float y, float _detectionRange, Player& _player)
    : Enemy(x, y, 0.0f, 1000), detectionRange(_detectionRange), player(_player),
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


    bossName = "Agis The World Ender";

    if (!bossFont.loadFromFile("assets/fonts/American_Captain.ttf")) {
        cerr << "Erreur : Impossible de charger la police du Boss !" << endl;
    }
    if (!laserProjectileTexture.loadFromFile("assets/enemy/boss/projectile.png")) {
        cerr << "Erreur chargement TEXTURE PROJECTILE LASER !" << endl;
    }
    if (!meteorProjectileTexture.loadFromFile("assets/enemy/boss/meteorProjectile.png")) {
        cerr << "Erreur chargement TEXTURE PROJECTILE METEOR !" << endl;
    }
    if (!chargeProjectileTexture.loadFromFile("assets/enemy/boss/charge.png")) {
        cerr << "Erreur chargement TEXTURE PROJECTILE CHARGE !" << endl;
    }
    bossNameText.setFont(bossFont);
    bossNameText.setCharacterSize(30);
    bossNameText.setScale(.3f, .3f);
    bossNameText.setFillColor(Color::White);
    bossNameText.setOutlineColor(Color::Black);
    bossNameText.setOutlineThickness(2);
    bossNameText.setString(bossName);

    loadTextures();
    initializeBossHealthBar();
    shape.setTexture(nullptr);
    shape.setPosition(x, y);
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

    //hitbox
    hitbox.setSize(Vector2f(40.f, 142.5f));
    hitbox.setFillColor(Color(255, 0, 0, 100));
    hitbox.setOutlineColor(Color::Transparent);
    hitbox.setOutlineThickness(0.f);
    hitboxOffset = Vector2f(-20.f, 150.f);
}

/* ====================
    MISE À JOUR DU BOSS
   ==================== */
void BossEnemy::update(float deltaTime, const RenderWindow& window, const Vector2f& playerPosition, Map& map) {
    if (isDead) return;

    if (!playerEnteredArena) {
        checkPlayerEntry();
        return;
    }

    for (auto it = projectiles.begin(); it != projectiles.end();) {
        it->update(deltaTime);

        if (!map.isWalkable(it->shape.getPosition(), Vector2f(10, 10), it->shape.getGlobalBounds())) {
            it = projectiles.erase(it);
        }
        else {
            ++it;
        }
    }

    for (auto& enemy : chaserEnemies) {
        enemy->update(deltaTime, window, player.getPosition(), map);
    }

    for (auto& enemy : patternEnemies) {
        enemy->update(deltaTime, window, player.getPosition(), map);
    }

    checkProjectileCollision();
    checkPlayerAttack();
    /*checkPlayerCollision();*/
    applySmoothPushback(deltaTime);
    updateHitbox();

    if (Keyboard::isKeyPressed(Keyboard::N)) reduceHealth(10);

    updateBossHealthBarPosition(player.getCameraView());

    switch (currentPhase) {
    case BossPhase::INTRO_DIALOGUE: handleIntroDialogue(); break;
    case BossPhase::SPAWN: handleSpawnAnimation(); break;
    case BossPhase::IDLE: handleIdlePhase(); break;
    case BossPhase::ATTACKING: handleAttackingPhase(deltaTime); break;
    case BossPhase::DEAD:
        handleDeathPhase();
        animate(deltaTime);
        return;
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
    if (currentPhase == BossPhase::DEAD) return;
    if (!isAttacking) return;

    executePattern(deltaTime);

    if (currentFrame >= totalFrames - 1) {
        cout << "Animation d'attaque terminée, passage en IDLE." << endl;
        currentFrame = 0;
        currentPhase = BossPhase::IDLE;
        setTexture(idleTexture, 320, 320, 15, 0.1f);
        isAttacking = false;
        phaseClock.restart();
    }
}

void BossEnemy::reduceHealth(float damage) {
    if (health <= 0) return;

    health -= damage;
    if (health < 0) health = 0;

    cout << "Boss prend " << damage << " dégâts. Vie restante : " << health << endl;

    updateBossHealthBar();

    if (health <= 0) {
        cout << "Boss vaincu ! Lancement de l'animation de mort..." << endl;
        startDeathAnimation();
    }
}

void BossEnemy::handleDeathPhase() {
    if (!isDying) return;

    if (phaseClock.getElapsedTime().asSeconds() >= 0.1f) {
        if (currentFrame < 64) {
            animate(0.1f);
            phaseClock.restart();
        }
        else {
            cout << "Le Boss est définitivement mort et disparaît." << endl;
            bossHealthBarVisible = false;
            isDead = true;
        }
    }
}

void BossEnemy::applySmoothPushback(float deltaTime) {
    if (!isPushingBack) return;

    float elapsed = pushbackClock.getElapsedTime().asSeconds();
    float pushDuration = 0.5f;
    float factor = 1.0f - (elapsed / pushDuration);

    if (factor <= 0) {
        isPushingBack = false;
        return;
    }

    Vector2f pushStep = pushbackDirection * (pushbackStrength * factor * deltaTime);
    player.setPosition(player.getPosition() + pushStep);
}


void BossEnemy::pushPlayerBack() {
    Vector2f bossPos = getPosition();
    Vector2f playerPos = player.getPosition();
    Vector2f offSet = { 0, 200 };
    bossPos = bossPos + offSet;
    pushbackDirection = playerPos - bossPos ;

    float length = sqrt(pushbackDirection.x * pushbackDirection.x + pushbackDirection.y * pushbackDirection.y);
    if (length != 0) {
        pushbackDirection /= length;
    }

    pushbackStrength = 150.0f;
    isPushingBack = true;
    pushbackClock.restart();
}

void BossEnemy::updateHitbox() {
    hitbox.setPosition(getPosition() + hitboxOffset);
}

void BossEnemy::checkPlayerAttack() {
    if (damageCooldown.getElapsedTime().asSeconds() < damageCooldownTime) return;

    if (hitbox.getGlobalBounds().intersects(player.getAttackHitbox()) && player.playerAttacking()) {
        reduceHealth(20);
        pushPlayerBack();
        cout << "Le Boss a ete touche par l'attaque du joueur !" << endl;
        damageCooldown.restart();
    }
}

//void BossEnemy::checkPlayerCollision() {
//    if (hitbox.getGlobalBounds().intersects(player.getShape().getGlobalBounds())) {
//        cout << "🚨 Le joueur est entré en collision avec le Boss !" << endl;
//
//        pushPlayerBack();
//        player.reduceHealth(10);
//
//    }
//}

/* ====================
    ANIMATIONS
   ==================== */
void BossEnemy::initializeBossHealthBar() {
    maxHealth = health;

    //Contour de la barre de vie
    bossHealthBarOutline.setSize(Vector2f(270, 6));
    bossHealthBarOutline.setFillColor(Color::Transparent);
    bossHealthBarOutline.setOutlineColor(Color::Black);
    bossHealthBarOutline.setOutlineThickness(1);
    bossHealthBarOutline.setPosition(150, 800);

    //Barre de vie rouge
    bossHealthBar.setSize(Vector2f(270, 6));
    bossHealthBar.setFillColor(Color(130, 0, 0, 255));
    bossHealthBar.setPosition(150, 800);

    bossHealthBarVisible = false;
}

void BossEnemy::updateBossHealthBar() {
    float healthPercentage = health / maxHealth;
    bossHealthBar.setSize(Vector2f(270 * healthPercentage, 6));
}

void BossEnemy::updateBossHealthBarPosition(const View& cameraView) {
    Vector2f viewCenter = cameraView.getCenter();
    Vector2f viewSize = cameraView.getSize();

    float healthBarY = viewCenter.y + (viewSize.y / 2) - 40;

    float healthBarX = viewCenter.x - (bossHealthBarOutline.getSize().x / 2);

    bossHealthBar.setPosition(healthBarX, healthBarY);
    bossHealthBarOutline.setPosition(healthBarX, healthBarY);

    bossNameText.setPosition(healthBarX,
        healthBarY - 11);
}


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

        //Active la barre de vie du boss
        bossHealthBarVisible = true;
    }
}

void BossEnemy::startDeathAnimation() {
    if (isDying) return;

    currentPhase = BossPhase::DEAD;
    currentFrame = 0;
    isDying = true;

    setTexture(deathTexture, 320, 320, 65, 0.15f); 
    phaseClock.restart();

    cout << "Animation de mort en cours..." << endl;
    bossMusic.stop();
}

/* ====================
    GESTION DES ATTAQUES
   ==================== */
void BossEnemy::changePattern() {
    if (isAttacking) return;

    int randPattern = rand() % 4;
    currentFrame = 0;

    switch (randPattern) {
    case 0: currentPattern = BossPattern::LASER; setTexture(laserTexture, 320, 320, 42, 0.1f); attackDuration = 4.2f; break;
    case 1: currentPattern = BossPattern::SUMMON; setTexture(summonTexture, 320, 320, 70, 0.1f); attackDuration = 7.f; break;
    case 2: currentPattern = BossPattern::METEOR; setTexture(meteorTexture, 320, 320, 42, 0.1f); attackDuration = 4.2f; break;
    case 3: currentPattern = BossPattern::CHARGE; setTexture(chargeTexture, 320, 320, 42, 0.1f); attackDuration = 4.2f; break;
    }

    isAttacking = true;
    attackClock.restart();
}

void BossEnemy::executePattern(float deltaTime) {
    if (currentPhase == BossPhase::DEAD) return;
    if (!isAttacking) return;

    switch (currentPattern) {
    case BossPattern::LASER:
        currentPattern = BossPattern::LASER;
        launchLaserAttack();
        break;
    case BossPattern::SUMMON:
        launchSummonAttack();
        break;
    case BossPattern::METEOR:
        launchMeteorAttack();
        break;
    case BossPattern::CHARGE:
        launchChargeAttack();
    }

    if (attackClock.getElapsedTime().asSeconds() > attackDuration) {
        cout << "Fin de l'attaque, retour en IDLE." << endl;
        isAttacking = false;
        currentPhase = BossPhase::IDLE;
        setTexture(idleTexture, 320, 320, 15, 0.1f);
        phaseClock.restart();
    }
}

void BossEnemy::launchLaserAttack() {
    int numProjectiles = 10;
    float angleStep = 360.f / numProjectiles;
    float speed = 200.f;
    Vector2f spawnOffset(0, 200);

    for (int wave = 0; wave < 5; ++wave) {
        for (int i = 0; i < numProjectiles; ++i) {
            float angle = i * angleStep * (3.14159265f / 180.f);
            Vector2f direction(cos(angle), sin(angle));

            Vector2f projectilePosition = getPosition() + spawnOffset;
            RectangleShape projectile(Vector2f(10, 10));

            projectile.setTexture(&laserProjectileTexture);
            projectile.setPosition(projectilePosition);

            float rotationAngle = atan2(direction.y, direction.x) * (180.f / 3.14159265f);

            projectile.setRotation(rotationAngle);
            projectiles.emplace_back(projectile, direction, speed);
        }
    }
}

void BossEnemy::checkProjectileCollision() {
    for (auto it = projectiles.begin(); it != projectiles.end();) {
        if (it->shape.getGlobalBounds().intersects(player.getShape().getGlobalBounds())) {
            cout << "Le joueur a ete touché par un projectile !" << endl;
            player.reduceHealth(15);
            it = projectiles.erase(it);
        }
        else {
            ++it;
        }
    }
}

void BossEnemy::launchMeteorAttack() {
    int numProjectiles = 6;
    float angleStep = 360.f / numProjectiles;
    float speed = 150.f;
    Vector2f spawnOffset(0, 200);
    static Clock meteorWaveClock;
    static int currentWave = 0;

    if (meteorWaveClock.getElapsedTime().asSeconds() > 1.f && currentWave < 3) {
        for (int i = 0; i < numProjectiles; ++i) {
            float angle = i * angleStep * (3.14159265f / 180.f);
            Vector2f direction(cos(angle), sin(angle));

            Vector2f projectilePosition = getPosition() + spawnOffset;

            RectangleShape projectile(Vector2f(12, 12));
            projectile.setTexture(&meteorProjectileTexture);
            projectile.setScale(1.5f, 1.5f);
            projectile.setPosition(projectilePosition);

            float rotationAngle = atan2(direction.y, direction.x) * (180.f / 3.14159265f);
            projectile.setRotation(rotationAngle);

            projectiles.emplace_back(projectile, direction, speed);
        }
        currentWave++;
        meteorWaveClock.restart();
    }
}

void BossEnemy::launchSummonAttack() {
    static Clock summonWaveClock;
    static int currentWave = 0;
    int numEnemiesPerWave = 3;
    int totalWaves = 3;

    float spawnRadius = 100.f;

    if (summonWaveClock.getElapsedTime().asSeconds() > 1.0f && currentWave < totalWaves) {
        for (int i = 0; i < numEnemiesPerWave; ++i) {
            float angle = (rand() % 360) * (3.14159265f / 180.f);
            float offsetX = cos(angle) * spawnRadius;
            float offsetY = sin(angle) * spawnRadius;

            Vector2f spawnPosition = getPosition() + Vector2f(offsetX, offsetY);

            if (rand() % 2 == 0) {
                patternEnemies.push_back(make_unique<PatternEnemy>(
                    spawnPosition.x, spawnPosition.y + 200, 10.0f, 100.f, 0.f, player));
            }
            else {
                chaserEnemies.push_back(make_unique<ChaserEnemy>(
                    spawnPosition.x, spawnPosition.y + 200, 50.0f, 100, 150.0f, player));
            }
        }

        cout << "Boss invoque " << numEnemiesPerWave << " ennemis (Vague " << currentWave + 1 << ")" << endl;
        currentWave++;
        summonWaveClock.restart();
    }
}

void BossEnemy::launchChargeAttack() {
    int numProjectiles = 1200;
    float angleStep = 0.3f;
    float speed = 300.f;
    Vector2f spawnOffset(0, 290);


    if (currentFrame == 38) {
        for (int i = 0; i < numProjectiles; ++i) {
            float angle = i * angleStep * (3.14159265f / 180.f);
            Vector2f direction(cos(angle), sin(angle));

            Vector2f projectilePosition = getPosition() + spawnOffset;

            RectangleShape projectile(Vector2f(8, 8));
            projectile.setTexture(&chargeProjectileTexture);
            projectile.setPosition(projectilePosition);

            float rotationAngle = atan2(direction.y, direction.x) * (180.f / 3.14159265f);
            projectile.setRotation(rotationAngle);

            projectiles.emplace_back(projectile, direction, speed);
        }
        cout << "CHARGE : 1200 projectiles lancés !" << endl;
    }
}

float BossEnemy::getBossHealth() {
    return health;
}

void BossEnemy::draw(RenderWindow& window) {
    if (isDead) return;
    for (const auto& projectile : projectiles) {
        window.draw(projectile.shape);
    }
    for (auto& enemy : chaserEnemies) {
        enemy->draw(window);
    }
    for (auto& enemy : patternEnemies) {
        enemy->draw(window);
    }
    window.draw(shape);
    dialogue.draw(window);
    if (bossHealthBarVisible) {
        window.draw(bossHealthBarOutline);
        window.draw(bossHealthBar);
        window.draw(bossNameText);
    }
}


