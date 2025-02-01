#ifndef ENEMY_H
#define ENEMY_H

#include "Entity.h"
#include <SFML/Graphics.hpp>
#include <iostream>

using namespace sf;
using namespace std;

class Enemy : public Entity {
private:
    Vector2f position;
    float speed;

public:
    //Constructeur de l'ennemi
    Enemy(float x, float y, float _speed);

    //Définit la position de l'ennemi
    void setPosition(const Vector2f& newPosition);

    //Retourne la forme de l'ennemi
    const RectangleShape& getShape() const;

    //Retourne la position actuelle de l'ennemi
    Vector2f getPosition() const;

    //Met à jour l'ennemi
    void update(float deltaTime, const RenderWindow& window, const Vector2f& playerPosition, Map& map) override;

    //Dessine l'ennemi à l'écran
    void draw(RenderWindow& window) override;

    //Destructeur virtuel
    virtual ~Enemy() {}
};

#endif
