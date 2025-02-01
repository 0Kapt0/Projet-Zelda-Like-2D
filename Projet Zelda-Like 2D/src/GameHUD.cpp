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
        playerIcon.setScale(.3f, .3f);  // Ajuste la taille de l’icône
        playerIcon.setPosition(10, 10);  // Position en bas de la barre de vie
        playerIcon.setColor(Color(150, 150, 150, 255));
    }

    //Créer un contour pour l'icône du joueur
    playerIconOutline.setSize(Vector2f(playerIcon.getGlobalBounds().width, playerIcon.getGlobalBounds().height));
    playerIconOutline.setFillColor(Color::Transparent);  // Pas de couleur interne
    playerIconOutline.setOutlineColor(Color(139, 124, 46, 255));     // Contour blanc
    playerIconOutline.setOutlineThickness(3);            // Épaisseur du contour
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
    float healthPercent = player.getHealth() / 100.0f;  // Pourcentage de vie restant
    healthBar.setSize(Vector2f(200 * healthPercent, 20));  // Ajuste la largeur de la barre de vie
}

void GameHUD::draw(RenderWindow& window) {
    window.draw(healthBarOutline);
    window.draw(healthBar);
    window.draw(playerIconOutline);
    window.draw(playerIcon);  //Dessine l’icône du joueur sous la barre de vie
}

