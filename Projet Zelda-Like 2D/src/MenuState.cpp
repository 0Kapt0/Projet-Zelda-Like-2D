#include "../include/MenuState.h"
#include <iostream>

using namespace sf;
using namespace std;

MenuState::MenuState(RenderWindow& window)
    : State(window), selectedItemIndex(0) {
    if (!font.loadFromFile("assets/fonts/American_Captain.ttf")) {
        cerr << "Erreur de chargement de la police" << endl;
    }

    initMenu();
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

    menuOptions[selectedItemIndex].setFillColor(Color::Red);
}

void MenuState::handleInput() {
    if (Keyboard::isKeyPressed(Keyboard::Up)) {
        if (selectedItemIndex > 0) {
            menuOptions[selectedItemIndex].setFillColor(Color::White);
            selectedItemIndex--;
            menuOptions[selectedItemIndex].setFillColor(Color::Red);
        }
    }

    if (Keyboard::isKeyPressed(Keyboard::Down)) {
        if (selectedItemIndex < menuOptions.size() - 1) {
            menuOptions[selectedItemIndex].setFillColor(Color::White);
            selectedItemIndex++;
            menuOptions[selectedItemIndex].setFillColor(Color::Red);
        }
    }

    if (Keyboard::isKeyPressed(Keyboard::Enter)) {
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

void MenuState::update(float deltaTime) {
}

void MenuState::draw() {
    window.clear();
    for (auto& option : menuOptions) {
        window.draw(option);
    }
    window.display();
}
