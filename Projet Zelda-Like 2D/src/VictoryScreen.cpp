#include "../include/VictoryScreen.h"
#include <iostream>

using namespace sf;
using namespace std;

VictoryScreen::VictoryScreen(RenderWindow& window)
    : State(window), selectedItemIndex(-1) {

    if (!font.loadFromFile("assets/fonts/American_Captain.ttf")) {
        cerr << "Erreur de chargement de la police du Victory Screen !" << endl;
    }

    if (!backgroundTexture.loadFromFile("assets/backgrounds/victory_background.png")) {
        cerr << "Erreur de chargement du fond d'écran du Victory Screen !" << endl;
    }
    backgroundSprite.setTexture(backgroundTexture);
    backgroundSprite.setScale(
        window.getSize().x / backgroundSprite.getLocalBounds().width,
        window.getSize().y / backgroundSprite.getLocalBounds().height
    );

    if (!victoryMusic.openFromFile("assets/music/victory.ogg")) {
        cerr << "Erreur de chargement de la musique de victoire !" << endl;
    }
    else {
        victoryMusic.setLoop(true);
        victoryMusic.setVolume(20.f);
        victoryMusic.play();
    }

    initMenu();
}

VictoryScreen::~VictoryScreen() {
    victoryMusic.stop();
}

void VictoryScreen::initMenu() {
    Text victoryText("VICTORY", font, 80);
    victoryText.setFillColor(Color::Yellow);
    victoryText.setOutlineColor(Color::Black);
    victoryText.setOutlineThickness(5);
    victoryText.setPosition(window.getSize().x / 2 - 150, 50);
    menuOptions.push_back(victoryText);

    Text restart("Recommencer", font, 60);
    restart.setPosition(window.getSize().x / 2 - 100, 200);
    menuOptions.push_back(restart);

    Text quit("Quitter", font, 60);
    quit.setPosition(window.getSize().x / 2 - 100, 300);
    menuOptions.push_back(quit);

    for (size_t i = 1; i < menuOptions.size(); ++i) {
        menuOptions[i].setFillColor(Color::White);
    }
}

void VictoryScreen::handleInput() {
    Vector2i mousePos = Mouse::getPosition(window);
    bool mouseOverOption = false;

    for (size_t i = 1; i < menuOptions.size(); ++i) {
        if (menuOptions[i].getGlobalBounds().contains(mousePos.x, mousePos.y)) {
            menuOptions[i].setFillColor(Color::Green);
            mouseOverOption = true;

            if (Mouse::isButtonPressed(Mouse::Left)) {
                selectedItemIndex = i;
                switch (selectedItemIndex) {
                case 1:
                    cout << "Recommencer le jeu !" << endl;
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

void VictoryScreen::update(float deltaTime) {
    handleInput();
}

void VictoryScreen::draw() {
    window.clear();
    window.draw(backgroundSprite);
    for (auto& option : menuOptions) {
        window.draw(option);
    }
    window.display();
}
