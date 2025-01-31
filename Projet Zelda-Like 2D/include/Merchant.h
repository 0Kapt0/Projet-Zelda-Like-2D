#ifndef MERCHANT_H
#define MERCHANT_H

#include <SFML/Graphics.hpp>
#include "AlliedEntity.h"

using namespace sf;
using namespace std;

class Merchant : public AlliedEntity {
private:
    Texture merchantTex;

public:
    Merchant(float x, float y);
    void interact() override;
};

#endif
