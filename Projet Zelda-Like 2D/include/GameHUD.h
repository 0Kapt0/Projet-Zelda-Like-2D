#ifndef GAMEHUD_H
#define GAMEHUD_H

#include "HUD.h"
#include "Player.h"

class GameHUD : public HUD {
private:
    Player& player;

    RectangleShape healthBar;         // Barre de vie rouge
    RectangleShape healthBarOutline;  // Contour de la barre de vie
    Sprite playerIcon;                // Icône du joueur
    Texture playerIconTexture;        // Texture pour l'icône du joueur
    RectangleShape playerIconOutline;

public:
    GameHUD(Player& player);

    void update(float deltaTime) override;
    void draw(RenderWindow& window) override;
};

#endif

