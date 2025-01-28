#ifndef ENTITY_H
#define ENTITY_H

#include <SFML/Graphics.hpp>
using namespace sf;

class Entity {
protected:
    RectangleShape shape;
public:
    virtual void update(float deltaTime, const RenderWindow& window, const Vector2f& playerPosition) = 0;
    virtual void draw(sf::RenderWindow& window) = 0;
    virtual ~Entity() {}
    bool intersects(const Entity& other) const;
};

#endif

