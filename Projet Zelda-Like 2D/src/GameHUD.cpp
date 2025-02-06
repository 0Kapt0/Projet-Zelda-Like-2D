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
        playerIcon.setScale(.18f, .18f); 
        playerIcon.setPosition(20, 20);
        playerIcon.setColor(Color(150, 150, 150, 255));
    }

    //Charge la texture de l’icône blood du joueur
    if (!playerBloodTexture.loadFromFile("assets/HUD/blood.png")) {
        std::cerr << "Erreur de chargement de l'icône blood du joueur\n";
    }
    else {
        playerBlood.setTexture(playerBloodTexture);
        playerBlood.setScale(.025f, .025f);
        playerBlood.setPosition(20, 20);
        playerBlood.setColor(Color(255, 255, 255, 200));
    }

    //Charge la texture de la potion
    if (!potionTexture.loadFromFile("assets/HUD/health_potion.png")) {
        std::cerr << "Erreur de chargement de l'icône de la potion\n";
    }
    else {
        potionIcon.setTexture(potionTexture);
        potionIcon.setScale(3.5f, 3.5f);
        potionIcon.setPosition(20, 740);
    }

    //Cooldown de la potion (remplissage progressif)
    potionCooldownBar.setSize(Vector2f(70, 0));
    potionCooldownBar.setFillColor(Color(25, 25, 25, 180));
    potionCooldownBar.setPosition(45, 765);


    //Texte du nombre de potions
    potionCountText.setFont(font);
    potionCountText.setCharacterSize(20);
    potionCountText.setFillColor(Color(139, 124, 46, 255));
    potionCountText.setPosition(93, 765);

    //Fond du carré pour la potion
    potionHUD.setSize(Vector2f(70, 70));
    potionHUD.setFillColor(Color(25, 25, 25, 255));
    potionHUD.setPosition(45, 765);

    //Outline du carré
    potionHUDOutline.setSize(Vector2f(70, 70));
    potionHUDOutline.setFillColor(Color::Transparent);
    potionHUDOutline.setOutlineColor(Color(139, 124, 46, 255));
    potionHUDOutline.setOutlineThickness(3);
    potionHUDOutline.setPosition(45, 765);

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
    healthBarOutline.setPosition(160, 20);

    //Barre de vie
    healthBar.setSize(Vector2f(200, 20));
    healthBar.setFillColor(Color(100, 0, 0, 255));
    healthBar.setPosition(160, 20);
}

void GameHUD::update(float deltaTime) {
    //Gestion du cooldown visuel (disparition du carré)
    if (!player.canUsePotion()) {
        float cooldownProgress = player.getPotionCooldownTime() / 10.0f;
        potionCooldownBar.setSize(Vector2f(70, 70 * cooldownProgress));
        potionCooldownBar.setPosition(45, 765 + (70 * (1.0f - cooldownProgress)));
    }
    else {
        potionCooldownBar.setSize(Vector2f(70, 0));
    }

    // Nombre de potions disponibles
    potionCountText.setString("x" + std::to_string(player.getHealthPotions()));

    // Ajuste la barre de vie
    float healthPercent = player.getHealth() / 70.0f;
    healthBar.setSize(Vector2f(200 * healthPercent, 20));
    float healthBlood = 140 - player.getHealth() * 2;
    playerBlood.setColor(Color(255, 255, 255, healthBlood));
}


void GameHUD::draw(RenderWindow& window) {
    window.draw(healthBarOutline);
    window.draw(healthBar);
    window.draw(playerIconOutline);
    window.draw(playerIcon);
    window.draw(playerBlood);

    //Dessine le fond et l'outline du HUD de la potion
    window.draw(potionHUDOutline);
    window.draw(potionHUD);

    //Dessine l'icône de la potion et le nombre
    window.draw(potionIcon);
    window.draw(potionCountText);

    window.draw(potionCooldownBar);
}

