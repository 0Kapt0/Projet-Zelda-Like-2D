#ifndef STATE_H
#define STATE_H

#include <SFML/Graphics.hpp>

class State {
protected:
    sf::RenderWindow& window;

public:
    State(sf::RenderWindow& window) : window(window) {}
    virtual ~State() {}

    virtual void handleInput() = 0;
    virtual void update(float deltaTime) = 0;
    virtual void draw() = 0;
};

#endif
