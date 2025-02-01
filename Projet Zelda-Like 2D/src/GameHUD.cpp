#include "../include/GameHUD.h"
#include <iostream>

GameHUD::GameHUD(Player& player) : player(player) {
    if (!font.loadFromFile("assets/fonts/American_Captain.ttf")) {
        std::cerr << "Erreur de chargement de la police\n";
    }

    //Charge la texture de l’icône du joueur
    if (!playerIconTexture.loadFromFile("assets/HUD/player_icon.png")) {
        std::cerr << "Erreur de chargement de l'icône du joueur\n";
    }
    else {
        playerIcon.setTexture(playerIconTexture);
        playerIcon.setScale(.3f, .3f); 
        playerIcon.setPosition(10, 10);
        playerIcon.setColor(Color(150, 150, 150, 255));
    }

    //Créer un contour pour l'icône du joueur
    playerIconOutline.setSize(Vector2f(playerIcon.getGlobalBounds().width, playerIcon.getGlobalBounds().height));
    playerIconOutline.setFillColor(Color::Transparent);
    playerIconOutline.setOutlineColor(Color(139, 124, 46, 255));
    playerIconOutline.setOutlineThickness(3);
    playerIconOutline.setPosition(playerIcon.getPosition().x, playerIcon.getPosition().y);

    //Contour de la barre de vie
    healthBarOutline.setSize(Vector2f(200, 20));
    healthBarOutline.setFillColor(Color::Transparent);
    healthBarOutline.setOutlineColor(Color(139, 124, 46, 255));
    healthBarOutline.setOutlineThickness(2);
    healthBarOutline.setPosition(150, 20);

    //Barre de vie
    healthBar.setSize(Vector2f(200, 20));
    healthBar.setFillColor(Color(139, 0, 0, 255));
    healthBar.setPosition(150, 20);
}

void GameHUD::update(float deltaTime) {
    //Ajuste la bar de vie
    float healthPercent = player.getHealth() / 100.0f;
    healthBar.setSize(Vector2f(200 * healthPercent, 20));
}

void GameHUD::draw(RenderWindow& window) {
    window.draw(healthBarOutline);
    window.draw(healthBar);
    window.draw(playerIconOutline);
    window.draw(playerIcon);
}

