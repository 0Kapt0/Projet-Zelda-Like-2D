#ifndef MENUSTATE_H
#define MENUSTATE_H

#include "State.h"
#include <vector>
#include <SFML/Graphics.hpp>

class MenuState : public State {
private:
    std::vector<sf::Text> menuOptions;
    sf::Font font;
    int selectedItemIndex;

public:
    MenuState(sf::RenderWindow& window);
    virtual ~MenuState() {}

    void handleInput() override;
    void update(float deltaTime) override;
    void draw() override;

    void initMenu();
};

#endif
