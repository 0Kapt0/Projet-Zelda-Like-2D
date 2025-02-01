#ifndef GAMEHUD_H
#define GAMEHUD_H

#include "HUD.h"
#include "Player.h"

class GameHUD : public HUD {
private:
    Player& player;

    RectangleShape healthBar;
    RectangleShape healthBarOutline;
    Sprite playerIcon;
    Texture playerIconTexture;
    RectangleShape playerIconOutline;
    RectangleShape potionHUD;   // Fond du carr� pour afficher la potion
    RectangleShape potionHUDOutline; // Outline du carr� de la potion
    RectangleShape potionCooldownBar; // Barre de cooldown qui se remplit


    Sprite potionIcon;       // Ic�ne de la potion
    Texture potionTexture;   // Texture de l�ic�ne de potion
    Text potionCountText;    // Texte du nombre de potions

public:
    GameHUD(Player& player);

    void update(float deltaTime) override;
    void draw(RenderWindow& window) override;
};

#endif

