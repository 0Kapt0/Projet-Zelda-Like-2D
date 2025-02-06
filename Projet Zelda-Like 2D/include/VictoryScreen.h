#ifndef VICTORYSCREEN_H
#define VICTORYSCREEN_H

#include "State.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

using namespace sf;
using namespace std;

class VictoryScreen : public State {
private:
    vector<Text> menuOptions;
    Font font;
    Texture backgroundTexture;
    Sprite backgroundSprite;
    Music victoryMusic;
    int selectedItemIndex;

    void initMenu();

public:
    VictoryScreen(RenderWindow& window);
    ~VictoryScreen();

    void handleInput();
    void update(float deltaTime) override;
    void draw() override;
};

#endif
