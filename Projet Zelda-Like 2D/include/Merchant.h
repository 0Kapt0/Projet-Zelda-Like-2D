#ifndef MERCHANT_H
#define MERCHANT_H

#include <SFML/Graphics.hpp>
#include "AlliedEntity.h"

using namespace sf;
using namespace std;

class Merchant : public AlliedEntity {
private:
    static Texture merchantTex;

public:
    Merchant(float x, float y);
    static void loadMerchantTexture(const string& texturePath);
    void interact() override;
    void update(float deltaTime, const RenderWindow& window, const Vector2f& playerPosition, Map& map) override;
};

#endif
