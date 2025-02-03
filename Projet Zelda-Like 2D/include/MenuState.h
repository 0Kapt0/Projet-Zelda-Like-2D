#ifndef MENUSTATE_H
#define MENUSTATE_H

#include "State.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

using namespace sf;
using namespace std;

class MenuState : public State {
private:
    vector<Text> menuOptions;
    Font font;

    Music menuMusic;

    //Ajout du fond d'écran
    Texture backgroundTexture;
    Sprite backgroundSprite;

    void initMenu();
    void handleInput();
public:
    MenuState(RenderWindow& window);
    ~MenuState();

    int selectedItemIndex;

    void update(float deltaTime) override;
    void draw() override;
};

#endif
