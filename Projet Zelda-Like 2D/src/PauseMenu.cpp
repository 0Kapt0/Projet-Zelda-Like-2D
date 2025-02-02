#include "../include/PauseMenu.h"
#include <iostream>

using namespace sf;
using namespace std;

PauseMenu::PauseMenu(RenderWindow& window)
    : selectedItemIndex(-1) {

    if (!font.loadFromFile("assets/fonts/American_Captain.ttf")) {
        cerr << "Erreur de chargement de la police\n";
    }

    // Fond semi-transparent
    background.setSize(Vector2f(window.getSize().x, window.getSize().y));
    background.setFillColor(Color(0, 0, 0, 150)); // Noir semi-transparent

    // Titre "Pause"
    title.setFont(font);
    title.setString("Pause");
    title.setCharacterSize(50);
    title.setFillColor(Color::White);
    title.setPosition(window.getSize().x / 2 - 50, 100);

    // Options du menu
    vector<string> options = { "Continuer", "Quitter" };
    for (size_t i = 0; i < options.size(); ++i) {
        Text text;
        text.setFont(font);
        text.setString(options[i]);
        text.setCharacterSize(30);
        text.setFillColor(Color::White);
        text.setPosition(window.getSize().x / 2 - 50, 200 + i * 50);
        menuOptions.push_back(text);
    }
}

void PauseMenu::draw(RenderWindow& window) {
    window.draw(background);
    window.draw(title);
    for (auto& option : menuOptions) {
        window.draw(option);
    }
}

void PauseMenu::handleInput(RenderWindow& window, bool& isPaused) {
    Vector2i mousePos = Mouse::getPosition(window);

    for (int i = 0; i < menuOptions.size(); ++i) {
        if (menuOptions[i].getGlobalBounds().contains(mousePos.x, mousePos.y)) {
            menuOptions[i].setFillColor(Color::Red);

            if (Mouse::isButtonPressed(Mouse::Left)) {
                if (i == 0) {  // "Continuer"
                    isPaused = false;
                }
                else if (i == 1) {  // "Quitter"
                    window.close();
                }
            }
        }
        else {
            menuOptions[i].setFillColor(Color::White);
        }
    }
}
