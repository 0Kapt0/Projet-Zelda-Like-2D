#include "../include/Enemy.h"

using namespace sf;
using namespace std;

//Constructeur : initialise la position et la vitesse
Enemy::Enemy(float x, float y, float _speed)
    : position(x, y), speed(_speed) {
    shape.setPosition(position);
}

//Met à jour la position de l'ennemi et l'anime
void Enemy::update(float deltaTime, const RenderWindow& window, const Vector2f& playerPosition, Map& map) {
    animate(deltaTime);
    position.x += speed * deltaTime;

    //Vérifie que l'ennemi reste dans la fenêtre
    Vector2u windowSize = window.getSize();
    if (position.x < 0) position.x = 0;
    if (position.x > windowSize.x - shape.getSize().x) position.x = windowSize.x - shape.getSize().x;

    shape.setPosition(position);
}

//Définit une nouvelle position pour l'ennemi
void Enemy::setPosition(const Vector2f& newPosition) {
    position = newPosition;
    shape.setPosition(position);
}

//Retourne la forme de l'ennemi
const RectangleShape& Enemy::getShape() const {
    return shape;
}

//Retourne la position actuelle de l'ennemi
Vector2f Enemy::getPosition() const {
    return position;
}

//Dessine l'ennemi à l'écran
void Enemy::draw(RenderWindow& window) {
    window.draw(shape);
}
