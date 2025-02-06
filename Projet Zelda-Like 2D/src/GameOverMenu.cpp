#include "../include/GameOverMenu.h"
#include <iostream>

using namespace sf;
using namespace std;

GameOverMenu::GameOverMenu(RenderWindow& window)
    : State(window), selectedItemIndex(-1), isGameOver(false) {

    if (!font.loadFromFile("assets/fonts/American_Captain.ttf")) {
        cerr << "Erreur de chargement de la police du Game Over !" << endl;
    }

    if (!backgroundTexture.loadFromFile("assets/backgrounds/gameover_background.png")) {
        cerr << "Erreur de chargement du fond d'écran du Game Over !" << endl;
    }
    backgroundSprite.setTexture(backgroundTexture);
    backgroundSprite.setScale(
        window.getSize().x / backgroundSprite.getLocalBounds().width,
        window.getSize().y / backgroundSprite.getLocalBounds().height
    );

    if (!gameOverMusic.openFromFile("assets/music/gameover.ogg")) {
        cerr << "Erreur de chargement de la musique du Game Over !" << endl;
    }
    else {
        gameOverMusic.setLoop(true);
        gameOverMusic.setVolume(20.f);
    }

    initMenu();
}

GameOverMenu::~GameOverMenu() {
    gameOverMusic.stop();
}

void GameOverMenu::initMenu() {
    Text gameOverText("GAME OVER", font, 80);
    gameOverText.setFillColor(Color::Red);
    gameOverText.setOutlineColor(Color::Black);
    gameOverText.setOutlineThickness(5);
    gameOverText.setPosition(window.getSize().x / 2 - 150, 50);
    menuOptions.push_back(gameOverText);

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

void GameOverMenu::triggerGameOver() {
    isGameOver = true;
    gameOverMusic.play();
}

void GameOverMenu::handleInput() {
    if (!isGameOver) return;

    Vector2i mousePos = Mouse::getPosition(window);
    bool mouseOverOption = false;

    for (size_t i = 1; i < menuOptions.size(); ++i) {
        if (menuOptions[i].getGlobalBounds().contains(mousePos.x, mousePos.y)) {
            menuOptions[i].setFillColor(Color::Red);
            mouseOverOption = true;

            if (Mouse::isButtonPressed(Mouse::Left)) {
                selectedItemIndex = i;
                switch (selectedItemIndex) {
                case 1:
                    cout << "Recommencer le jeu !" << endl;
                    isGameOver = false;
                    gameOverMusic.stop();
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


void GameOverMenu::update(float deltaTime) {
    if (isGameOver) {
        handleInput();
    }
}

void GameOverMenu::draw() {
    if (!isGameOver) return;

    window.clear();
    window.draw(backgroundSprite);
    for (auto& option : menuOptions) {
        window.draw(option);
    }
    window.display();
}
