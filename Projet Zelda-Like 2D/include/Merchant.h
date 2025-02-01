#ifndef MERCHANT_H
#define MERCHANT_H

#include <SFML/Graphics.hpp>
#include "AlliedEntity.h"

using namespace sf;
using namespace std;

class Merchant : public AlliedEntity {
private:
    //Texture partagée du marchand
    static Texture merchantTex;

public:
    //Constructeur
    Merchant(float x, float y);

    //Charge la texture
    static void loadMerchantTexture(const string& texturePath);

    void interact() override; //Interaction avec le joueur
    void update(float deltaTime, const RenderWindow& window, const Vector2f& playerPosition, Map& map) override; //Met à jour l'entité
};

#endif
