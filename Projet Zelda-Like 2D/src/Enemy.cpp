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
}

//Retourne la forme de l'ennemi
const RectangleShape& Enemy::getShape() const {
    return shape;
}

void Enemy::setPosition(const Vector2f& newPosition) {
    position = newPosition;  // MAJ de position
    shape.setPosition(newPosition);
}

//Dessine l'ennemi à l'écran
void Enemy::draw(RenderWindow& window) {
    window.draw(shape);
}
