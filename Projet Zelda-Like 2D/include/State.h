#ifndef STATE_H
#define STATE_H

#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;

class State {
protected:
    RenderWindow& window;

public:
    State(RenderWindow& window) : window(window) {}
    virtual ~State() {}
    virtual void handleInput() = 0;
    virtual void update(float deltaTime) = 0;
    virtual void draw() = 0;
};

#endif
