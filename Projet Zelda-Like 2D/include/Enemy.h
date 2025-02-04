#ifndef ENEMY_H
#define ENEMY_H

#include "Entity.h"
#include <SFML/Graphics.hpp>

using namespace sf;

class Enemy : public Entity {
protected:
    Vector2f position;
    float speed;
    float health;
    float maxHealth;

    //Barre de vie
    RectangleShape healthBar;
    RectangleShape healthBarOutline;
    void initializeHealthBar();
    void updateHealthBar();

public:
    //Constructeur corrigé avec les bons paramètres
    Enemy(float x, float y, float _speed, float _health);

    //Getters
    const RectangleShape& getShape() const;
    Vector2f getPosition() const { return shape.getPosition(); }
    float getHealth() const { return health; }

    // Réduit la vie de l'ennemi
    void reduceHealth(float damage);

    //Setters
    void setPosition(const Vector2f& newPosition);

    //affichage et maj
    void update(float deltaTime, const RenderWindow& window, const Vector2f& playerPosition, Map& map) override;
    void draw(RenderWindow& window) override;

    //Destructeur virtuel pour éviter des erreurs de polymorphisme
    virtual ~Enemy() {}
};

#endif

