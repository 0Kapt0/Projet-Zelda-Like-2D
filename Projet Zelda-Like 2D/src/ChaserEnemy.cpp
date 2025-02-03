#include "../include/ChaserEnemy.h"
#include <cmath>
#include <iostream>

ChaserEnemy::ChaserEnemy(float x, float y, float _speed, float _detectionRange, Player& _player)
    : Enemy(x, y, _speed), detectionRange(_detectionRange), player(_player) {

    speed = _speed;

    if (!texture.loadFromFile("assets/enemy/necro/necro_anim.png")) {
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

void ChaserEnemy::update(float deltaTime, const RenderWindow& window, const Vector2f& playerPosition, Map& map) {
    Vector2f direction = playerPosition - getPosition();
    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    //Vérification de la normalisation
    if (distance > 0.0f) {
        direction /= distance;
    }

    //Déplacement si dans la portée
    if (distance < detectionRange) {
        Vector2f newPosition = getPosition() + (direction * speed * deltaTime);

        //Vérification de la collision
        if (map.isWalkable(newPosition, shape.getSize(), shape.getGlobalBounds())) {
            position = newPosition;
            shape.setPosition(position);
        }
    }

    if (shape.getGlobalBounds().intersects(player.getShape().getGlobalBounds())) {
        player.reduceHealth(10);
    }

    animate(deltaTime);
}





void ChaserEnemy::draw(RenderWindow& window) {
    window.draw(shape);
}
