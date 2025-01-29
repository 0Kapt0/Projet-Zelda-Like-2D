#include "../include/MenuState.h"
#include <SFML/Graphics.hpp>
#include <iostream>

MenuState::MenuState(sf::RenderWindow& window)
    : State(window), selectedItemIndex(0) {
    if (!font.loadFromFile("assets/fonts/American_Captain.ttf")) {
        std::cerr << "Erreur de chargement de la police" << std::endl;
    }

    initMenu();
}

void MenuState::initMenu() {
    sf::Text startGame("Start Game", font, 30);
    startGame.setPosition(100, 100);
    menuOptions.push_back(startGame);

    sf::Text options("Options", font, 30);
    options.setPosition(100, 150);
    menuOptions.push_back(options);

    sf::Text quit("Quit", font, 30);
    quit.setPosition(100, 200);
    menuOptions.push_back(quit);

    menuOptions[selectedItemIndex].setFillColor(sf::Color::Red);
}

void MenuState::handleInput() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
        if (selectedItemIndex > 0) {
            menuOptions[selectedItemIndex].setFillColor(sf::Color::White);
            selectedItemIndex--;
            menuOptions[selectedItemIndex].setFillColor(sf::Color::Red);
        }
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        if (selectedItemIndex < menuOptions.size() - 1) {
            menuOptions[selectedItemIndex].setFillColor(sf::Color::White);
            selectedItemIndex++;
            menuOptions[selectedItemIndex].setFillColor(sf::Color::Red);
        }
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
        switch (selectedItemIndex) {
        case 0:
            std::cout << "Start Game Selected" << std::endl;
            break;
        case 1:
            std::cout << "Options Selected" << std::endl;
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
