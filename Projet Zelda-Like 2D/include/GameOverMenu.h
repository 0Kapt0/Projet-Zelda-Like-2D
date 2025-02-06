#ifndef GAMEOVERMENU_H
#define GAMEOVERMENU_H

#include "State.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

using namespace sf;
using namespace std;

class GameOverMenu : public State {
private:
    Font font;
    vector<Text> menuOptions;
    Texture backgroundTexture;
    Sprite backgroundSprite;
    Music gameOverMusic;

    void initMenu();

public:
    GameOverMenu(RenderWindow& window);
    ~GameOverMenu();

    int selectedItemIndex;
    bool isGameOver;

    void handleInput();
    void triggerGameOver();
    void update(float deltaTime) override;
    void draw() override;
};

#endif
