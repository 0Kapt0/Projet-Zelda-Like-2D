#include "../include/Player.h"
#include "../include/Map.h"
#include <iostream>

using namespace sf;
using namespace std;

Player::Player()
    : speed(150.0f), position(400.0f, 400.0f), health(50),
    playerDead(false), isDying(false), isDashing(false),
    canDash(true), isMoving(false), healthPotions(0),
    isPotionOnCooldown(false) {

    //Chargement des sons de pas
    array<string, 6> footstepFiles = {
        "assets/player/sounds/footstep1.wav",
        "assets/player/sounds/footstep2.wav",
        "assets/player/sounds/footstep3.wav",
        "assets/player/sounds/footstep4.wav",
        "assets/player/sounds/footstep5.wav",
        "assets/player/sounds/footstep6.wav"
    };
    for (size_t i = 0; i < footstepFiles.size(); i++) {
        if (!footstepBuffers[i].loadFromFile(footstepFiles[i]))
            cerr << "Erreur chargement : " << footstepFiles[i] << endl;
        footsteps[i].setBuffer(footstepBuffers[i]);
        footsteps[i].setVolume(5.f);
    }

    //Chargement des sons d'attaque et de dash et de mort
    array<pair<string, SoundBuffer*>, 4> soundFiles = {
        make_pair("assets/player/sounds/swordSwing1.wav", &swordSwingBuffers[0]),
        make_pair("assets/player/sounds/swordSwing2.wav", &swordSwingBuffers[1]),
        make_pair("assets/player/sounds/playerDash1.wav", &playerDashBuffers[0]),
        make_pair("assets/player/sounds/playerDash2.wav", &playerDashBuffers[1])
    };

    for (auto& sound : soundFiles) {
        if (!sound.second->loadFromFile(sound.first))
            cerr << "Erreur chargement : " << sound.first << endl;
    }

    swordSwing[0].setBuffer(swordSwingBuffers[0]);
    swordSwing[1].setBuffer(swordSwingBuffers[1]);
    Dash[0].setBuffer(playerDashBuffers[0]);
    Dash[1].setBuffer(playerDashBuffers[1]);
    Dash[0].setVolume(15.f);
    Dash[1].setVolume(15.f);
    swordSwing[0].setVolume(30.f);
    swordSwing[1].setVolume(60.f);

    if (!playerDeathBuffer.loadFromFile("assets/player/sounds/death_sound.wav")) {
        cerr << "Erreur chargement : playerDeath.wav" << endl;
    }
    playerDeathSound.setBuffer(playerDeathBuffer);
    playerDeathSound.setVolume(10.f);
    playerDeathSound.setPitch(0.7f);


    //Chargement des textures du joueur
    array<pair<string, Texture*>, 6> textureFiles = {
        make_pair("assets/player/player_run.png", &playerRun),
        make_pair("assets/player/player_idle.png", &playerIdle),
        make_pair("assets/player/player_death.png", &playerDeath),
        make_pair("assets/player/player_dash.png", &playerDash),
        make_pair("assets/player/player_attack1.png", &playerAttack1),
        make_pair("assets/player/player_damage.png", &playerHit)
    };

    for (auto& texture : textureFiles) {
        if (!texture.second->loadFromFile(texture.first))
            cerr << "Texture non chargee : " << texture.first << endl;
    }

    //Initialisation de la taille et de la caméra
    playerSize = { 12, 16 };
    hitbox.setSize({ 12, 16 });
    hitbox.setFillColor(Color::Transparent);
    cameraView.setSize(426.67f, 320);
    cameraView.setCenter(position);

    //contructeur attack hitbox
    attackHitbox.setSize(Vector2f(42, 21));
    attackHitbox.setFillColor(Color(255, 0, 0, 100));
    attackHitbox.setOutlineThickness(1);
    attackHitbox.setOutlineColor(Color::Red);
    attackHitbox.setPosition(position);

}

//Gestion des entrées du joueur

void Player::handleInput(float deltaTime, Map& map) {
    Vector2f mouv(0.0f, 0.0f);

    if (!isDying) {
        if (Keyboard::isKeyPressed(Keyboard::Q) || Keyboard::isKeyPressed(Keyboard::A)) mouv.x -= 1.0f, shape.setScale(-1, 1);
        if (Keyboard::isKeyPressed(Keyboard::D)) mouv.x += 1.0f, shape.setScale(1, 1);
        if (Keyboard::isKeyPressed(Keyboard::Z) || Keyboard::isKeyPressed(Keyboard::W)) mouv.y -= 1.0f;
        if (Keyboard::isKeyPressed(Keyboard::S)) mouv.y += 1.0f;
        if (Keyboard::isKeyPressed(Keyboard::R)) reduceHealth(10);
        if (Keyboard::isKeyPressed(Keyboard::H)) useHealthPotion();
    }

    if (mouv.x != 0 || mouv.y != 0) {
        float length = sqrt(mouv.x * mouv.x + mouv.y * mouv.y);
        mouv /= length;
        isMoving = true;
    }
    else {
        isMoving = false;
    }

    velocity = mouv * speed;
    Vector2f newPositionX = { position.x + velocity.x * deltaTime, position.y };
    Vector2f newPositionY = { position.x, position.y + velocity.y * deltaTime };

    FloatRect newHitboxBoundsX = hitbox.getGlobalBounds();
    newHitboxBoundsX.left += velocity.x * deltaTime;

    FloatRect newHitboxBoundsY = hitbox.getGlobalBounds();
    newHitboxBoundsY.top += velocity.y * deltaTime;

    if (map.isWalkable(newPositionX, playerSize, newHitboxBoundsX)) {
        position.x = newPositionX.x;
        hitbox.setPosition(position.x - 9, position.y + 4);
    }

    if (map.isWalkable(newPositionY, playerSize, newHitboxBoundsY)) {
        position.y = newPositionY.y;
        hitbox.setPosition(position.x - 9, position.y + 4);
    }

    shape.setPosition(position);
}


/* ============================
        COMBAT & ATTAQUES
============================ */

//Attaque du joueur
void Player::playerAttack() {
    checkAttackDuration();

    if (!isAttacking && Keyboard::isKeyPressed(Keyboard::Space)) {
        isAttacking = true;
        currentFrame = 0;
        speed = 100;
        setTexture(playerAttack1, 64, 32, 4, 0.1f);
        attackDurationClock.restart();

        updateAttackHitbox();

        int index = rand() % 2;
        swordSwing[index].play();
    }
}

void Player::updateAttackHitbox() {
    Vector2f attackOffset(0, 12);

    float direction = shape.getScale().x;

    if (direction > 0) {
        attackOffset.x = 0.f;
    }
    else {
        attackOffset.x = -50.f;
    }

    attackHitbox.setPosition(position + attackOffset);

    if (direction > 0) {
        attackHitbox.move(7.f, 0.f);
    }
}



void Player::checkAttackDuration() {
    if (isAttacking && attackDurationClock.getElapsedTime().asSeconds() > 0.3f) {
        isAttacking = false;
    }
}

//Joue un son de pas à intervalle régulier
void Player::playFootstep() {
    if (footstepClock.getElapsedTime().asSeconds() > 0.4f) {
        int index = rand() % 6;
        footsteps[index].play();
        footstepClock.restart();
    }
}

/* ============================
          DÉPLACEMENT
============================ */

//Dash
void Player::dash() {
    if (Keyboard::isKeyPressed(Keyboard::LShift) && !isDashing && canDash) {
        int index = rand() % 2;
        Dash[index].play();
        isDashing = true;
        canDash = false;
        dashClock.restart();
        dashCooldownClock.restart();
        speed *= 2.5f;
    }

    if (isDashing) {
        setTexture(playerDash, 32, 32, 8, 0.1f);
        if (dashClock.getElapsedTime().asSeconds() > 0.2f) {
            isDashing = false;
            speed /= 2.5f;
        }
    }

    if (!canDash && dashCooldownClock.getElapsedTime().asSeconds() > 2.0f) {
        canDash = true;
    }
}

//Animation du joueur en mouvement ou à l'arrêt
void Player::playerWalk() {
    if (isDying || isAttacking) return;

    if (isMoving && !isDashing) {
        speed = 150.f;
        setTexture(playerRun, 32, 32, 8, 0.1f);
        playFootstep();
    }
    else if (!isDashing) {
        setTexture(playerIdle, 32, 32, 8, 0.1f);
    }
}

/* ============================
          MORT DU JOUEUR
============================ */

//Gestion de la mort du joueur
void Player::playerDie() {
    if (!isDying) {
        isDying = true;
        isMoving = false;
        isAttacking = false;
        speed = 0;
        currentFrame = 0;

        playerDeathSound.play();
    }

    if (currentFrame < 13) {
        setTexture(playerDeath, 32, 32, 14, 0.1f);
        if (cameraView.getSize().x > 200.f) {
            cameraView.zoom(0.75f);
        }
    }
    else {
        cout << "Le joueur est mort définitivement." << endl;
        playerDead = true;
        isDying = false;
    }
}


//Vérification de l'état du joueur (mort ou vivant)
void Player::handleDeath() {
    if (isDying) {
        playerDie();
    }

    if (playerDead) {
        return;
    }
}

/* ============================
        MISE À JOUR
============================ */

void Player::update(float deltaTime, const RenderWindow& window, const Vector2f& playerPosition, Map& map) {
    handleDeath();
    if (playerDead) return;

    updateDamageFlash();
    dash();
    handleInput(deltaTime, map);
    playerAttack();
    playerWalk();
    updatePotionCooldown();

    hitbox.setPosition(position.x - 6, position.y +16);
    animate(deltaTime);

    shape.setPosition(position);
    cameraView.setCenter(position);
    const_cast<RenderWindow&>(window).setView(cameraView);
}

/* ============================
        GESTION DES POTIONS
============================ */

//Récupération d'une potion
void Player::collectHealthPotion() {
    healthPotions++;
    cout << "Potion de vie recuperee ! Nombre actuel : " << healthPotions << endl;
}

//Utilisation d'une potion de soin
void Player::useHealthPotion() {
    if (healthPotions > 0 && health < 70 && !isPotionOnCooldown) {
        health += 30;
        if (health > 70) health = 70;
        healthPotions--;
        isPotionOnCooldown = true;
        potionCooldown.restart();
        cout << "Potion utilisee ! Cooldown en cours..." << endl;
    }
    else if (isPotionOnCooldown) {
        cout << "Potion en cooldown, veuillez attendre " << getPotionCooldownTime() << "s." << endl;
    }
    else {
        cout << "Aucune potion disponible ou vie deja pleine !" << endl;
    }
}

//Vérifie si le cooldown de la potion est terminé
bool Player::canUsePotion() const {
    return !isPotionOnCooldown || potionCooldown.getElapsedTime().asSeconds() >= 10.0f;
}

//Temps restant du cooldown de la potion
float Player::getPotionCooldownTime() const {
    float timeLeft = 10.0f - potionCooldown.getElapsedTime().asSeconds();
    return (timeLeft > 0) ? timeLeft : 0;
}

//Mise à jour du cooldown des potions
void Player::updatePotionCooldown() {
    if (isPotionOnCooldown && potionCooldown.getElapsedTime().asSeconds() >= 10.0f) {
        isPotionOnCooldown = false;
        cout << "Cooldown termine, potion disponible !" << endl;
    }
}

//Getter nombre de potions restantes
int Player::getHealthPotions() const {
    return healthPotions;
}

/* ============================
        AFFICHAGE
============================ */

//Affichage du joueur et de sa hitbox
void Player::draw(RenderWindow& window) {
    window.draw(shape);
    window.draw(hitbox);
    /*if (isAttacking) {
        window.draw(attackHitbox);
    }*/
}

/* ============================
        AUTRES MÉTHODES
============================ */

//Réduction de la santé du joueur
void Player::reduceHealth(int damage) {
    if (damageCooldown.getElapsedTime().asSeconds() < 1.f) {
        return;
    }

    damageCooldown.restart();

    health -= damage;
    cout << "Vie restante : " << health << endl;

    // Lancer le clignotement
    startDamageFlash();

    if (health <= 0 && !isDying) {
        cout << "Le joueur commence à mourir !" << endl;
        playerDie();
    }
}

void Player::startDamageFlash() {
    isTakingDamage = true;
    damageFlashCount = 0;
    damageFlashTimer.restart();
}

void Player::updateDamageFlash() {
    if (!isTakingDamage) return;

    if (damageFlashTimer.getElapsedTime().asSeconds() > 0.1f) {
        if (damageFlashCount % 2 == 0) {
            shape.setFillColor(Color::White);
        }
        else {
            shape.setFillColor(Color::Transparent);
        }

        damageFlashCount++;
        damageFlashTimer.restart();

        if (damageFlashCount >= 9) {
            isTakingDamage = false;
            shape.setFillColor(Color::White);
        }
    }
}

//Initialisation de la position du joueur sur la carte
void Player::initialize(Map& map) {
    setPosition(map.getPlayerStartPosition());
}

//Modifier position joueur
void Player::setPosition(const Vector2f& newPosition) {
    position = newPosition;
    shape.setPosition(position);
}

//Modifier vitesse joueur
void Player::setSpeed(float newSpeed) {
    speed = newSpeed;
}

//Getter la position du joueur
Vector2f Player::getPosition() const {
    return position;
}

//Getter la vitesse du joueur
float Player::getSpeed() const {
    return speed;
}

//Getter la santé du joueur
float Player::getHealth() const {
    return health;
}

//Getter caméra joueur
View Player::getCameraView() const {
    return cameraView;
}

bool Player::getIsDashing() const {
    return isDashing;
}

bool Player::playerAttacking() const {
    return isAttacking;
}

FloatRect Player::getAttackHitbox() const {
    return attackHitbox.getGlobalBounds();
}
