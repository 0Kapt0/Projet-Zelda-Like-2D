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

public:
    GameHUD(Player& player);

    void update(float deltaTime) override;
    void draw(RenderWindow& window) override;
};

#endif

