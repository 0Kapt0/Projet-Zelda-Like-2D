#ifndef MENUSTATE_H
#define MENUSTATE_H

#include "State.h"
#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;

class MenuState : public State {
private:
    vector<Text> menuOptions;
    int selectedItemIndex;
    Font font;

    void initMenu();
    void handleInput();

public:
    MenuState(RenderWindow& window);

    int getSelectedItemIndex() const { return selectedItemIndex; }

    void update(float deltaTime) override;
    void draw() override;
};

#endif

