#include "../include/PatternEnemy.h"
#include <cmath>
#include <iostream>

PatternEnemy::PatternEnemy(float x, float y, float _speed)
    : Enemy(x, y, _speed) {

    speed = _speed;

    if (!texture.loadFromFile("assets/enemy/slime/walk.png")) {
        std::cerr << "Erreur de chargement de la texture du ChaserEnemy !" << std::endl;
    }
    else {
        shape.setTexture(&texture);
        setTexture(texture, 14, 17, 4, 0.2f);
    }

    //Positionne l'ennemi au bon endroit
    setPosition(Vector2f(x, y));
    shape.setOrigin(shape.getSize().x / 2, shape.getSize().y / 2);
}

void PatternEnemy::update(float deltaTime, const RenderWindow& window, const Vector2f& playerPosition, Map& map) {
    animate(deltaTime);
}





void PatternEnemy::draw(RenderWindow& window) {
    window.draw(shape);
}
