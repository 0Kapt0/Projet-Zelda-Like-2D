#include "../include/PatternEnemy.h"
#include <cmath>
#include <iostream>

PatternEnemy::PatternEnemy(float x, float y, float _speed)
    : Enemy(x, y, _speed) {

    speed = _speed;

    if (!texture.loadFromFile("assets/enemy/slime/slime_walk.png")) {
        std::cerr << "Erreur de chargement de la texture du ChaserEnemy !" << std::endl;
    }
    else {
        shape.setTexture(&texture);
        setTexture(texture, 32, 32, 7, 0.1f);
    }

    //Positionne l'ennemi au bon endroit
    setPosition(Vector2f(x, y));
    shape.setOrigin(shape.getSize().x / 2, shape.getSize().y / 2);

    direction.x = -10;
}

void PatternEnemy::update(float deltaTime, const RenderWindow& window, const Vector2f& playerPosition, Map& map) {

    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    Vector2f newPosition = getPosition() + (direction * speed * deltaTime);

    //Vérification de la collision
    if (map.isWalkable(newPosition, shape.getSize(), shape.getGlobalBounds())) {
        position = newPosition;
        shape.setPosition(position);
    }
    else {
        if (direction.x == -10) {
            shape.move(10, 0);
            direction.x = 10;
        }
        else if (direction.x == 10) {
            shape.move(-10, 0);
            direction.x = -10;
        }
        std::cout << "Mur touché !\n";
    }
    animate(deltaTime);
}





void PatternEnemy::draw(RenderWindow& window) {
    window.draw(shape);
}
