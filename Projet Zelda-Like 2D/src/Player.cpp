#include "../include/Player.h"
#include <iostream>
#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;

Player::Player() : speed(50.0f), position(100.0f, 100.0f), health(100) {
    if (!playerTexture.loadFromFile("assets/player/player_anim.png")) {
        cerr << "Erreur : Impossible de charger la texture du joueur !" << endl;
    }
    else {
        cout << "Texture du joueur chargée avec succès !" << endl;
    }

    setTexture(playerTexture, 16, 19, 4, 0.1f);
    shape.setSize(Vector2f(16, 19));  // On adapte la taille du sprite à celle d’un frame
    shape.setPosition(position);
}

void Player::handleInput() {
    Vector2f mouv(0.0f, 0.0f);

    if (Keyboard::isKeyPressed(Keyboard::Q)) mouv.x -= 1.0f;
    if (Keyboard::isKeyPressed(Keyboard::D)) mouv.x += 1.0f;
    if (Keyboard::isKeyPressed(Keyboard::Z)) mouv.y -= 1.0f;
    if (Keyboard::isKeyPressed(Keyboard::S)) mouv.y += 1.0f;

    float hypo = sqrt(mouv.x * mouv.x + mouv.y * mouv.y);
    if (hypo > 0.0f) {
        mouv /= hypo;
    }

    position += mouv * speed * 0.1f;
}

void Player::update(float deltaTime, const RenderWindow& window, const Vector2f& playerPosition) {
    handleInput();
    animate(deltaTime);

    Vector2u windowSize = window.getSize();
    float maxX = static_cast<float>(windowSize.x) - shape.getSize().x;
    float maxY = static_cast<float>(windowSize.y) - shape.getSize().y;

    position.x = max(0.f, min(position.x, maxX));
    position.y = max(0.f, min(position.y, maxY));

    shape.setPosition(position);
}

void Player::reduceHealth(int damage) {
    health -= damage;
    cout << "Vie restante : " << health << endl;
    if (health <= 0) {
        cout << "Le joueur est mort !" << endl;
    }
}

void Player::setPosition(const Vector2f& newPosition) {
    position = newPosition;
    shape.setPosition(position);
}

void Player::setSpeed(float newSpeed) {
    speed = newSpeed;
}

Vector2f Player::getPosition() const {
    return position;
}

float Player::getSpeed() const {
    return speed;
}

float Player::getHealth() const {
    return health;
}

void Player::draw(RenderWindow& window) {
    window.draw(shape);
}
