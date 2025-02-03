#ifndef PAUSEMENU_H
#define PAUSEMENU_H

#include <SFML/Graphics.hpp>
#include <vector>

using namespace sf;
using namespace std;

class PauseMenu {
private:
    Font font;
    Text title;
    vector<Text> menuOptions;
    int selectedItemIndex;

    RectangleShape background;

public:
    PauseMenu(RenderWindow& window);

    void draw(RenderWindow& window);
    void handleInput(RenderWindow& window, bool& isPaused);
};

#endif
