#ifndef GAMEHUD_H
#define GAMEHUD_H

#include "HUD.h"
#include "Player.h"

class GameHUD : public HUD {
private:
    Player& player;

    //Barre de vie
    RectangleShape healthBar;
    RectangleShape healthBarOutline;

    //Icône du joueur
    Sprite playerIcon;
    Texture playerIconTexture;
    RectangleShape playerIconOutline;

    //Potion (HUD)
    RectangleShape potionHUD;
    RectangleShape potionHUDOutline;
    RectangleShape potionCooldownBar;

    //Potion (affichage)
    Sprite potionIcon;
    Texture potionTexture;
    Text potionCountText;

public:
    //Constructeur
    GameHUD(Player& player);

    //Mise à jour & affichage
    void update(float deltaTime) override;
    void draw(RenderWindow& window) override;
};

#endif


