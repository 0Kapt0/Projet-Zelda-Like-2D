#include "../include/MenuState.h"
#include <iostream>

using namespace sf;
using namespace std;

MenuState::MenuState(RenderWindow& window)
    : State(window), selectedItemIndex(-1) {
    if (!font.loadFromFile("assets/fonts/American_Captain.ttf")) {
        cerr << "Erreur de chargement de la police" << endl;
    }

    if (!backgroundTexture.loadFromFile("assets/backgrounds/menu_background.png")) {
        cerr << "Erreur de chargement du fond d'écran du menu\n";
    }
    backgroundSprite.setTexture(backgroundTexture);
    backgroundSprite.setScale(
        window.getSize().x / backgroundSprite.getLocalBounds().width,
        window.getSize().y / backgroundSprite.getLocalBounds().height
    );

    if (!menuMusic.openFromFile("assets/music/Music_jeu.ogg")) {
        cerr << "Erreur de chargement du fond d'écran du menu\n";
    } else {
        menuMusic.setLoop(true);
        menuMusic.setVolume(10.f);
        menuMusic.play();
    }

    initMenu();
}

MenuState::~MenuState() {
    menuMusic.stop();
}


void MenuState::initMenu() {
    Text startGame("Start Game", font, 30);
    startGame.setPosition(100, 100);
    menuOptions.push_back(startGame);

    Text options("Options", font, 30);
    options.setPosition(100, 150);
    menuOptions.push_back(options);

    Text quit("Quit", font, 30);
    quit.setPosition(100, 200);
    menuOptions.push_back(quit);

    for (auto& option : menuOptions) {
        option.setFillColor(Color::White);
    }
}


void MenuState::handleInput() {
    Vector2i mousePos = Mouse::getPosition(window);

    bool mouseOverOption = false;

    for (int i = 0; i < menuOptions.size(); ++i) {
        if (menuOptions[i].getGlobalBounds().contains(mousePos.x, mousePos.y)) {
            menuOptions[i].setFillColor(Color::Red);
            mouseOverOption = true;

            if (Mouse::isButtonPressed(Mouse::Left)) {
                selectedItemIndex = i;
                switch (selectedItemIndex) {
                case 0:
                    cout << "Start Game Selected" << endl;
                    break;
                case 1:
                    cout << "Options Selected" << endl;
                    break;
                case 2:
                    window.close();
                    break;
                }
            }
        }
        else {
            menuOptions[i].setFillColor(Color::White);
        }
    }

    if (!mouseOverOption) {
        selectedItemIndex = -1;
    }
}

void MenuState::update(float deltaTime) {
}

void MenuState::draw() {
    window.clear();
    window.draw(backgroundSprite);
    for (auto& option : menuOptions) {
        window.draw(option);
    }
}
