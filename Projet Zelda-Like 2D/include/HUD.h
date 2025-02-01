#ifndef HUD_H
#define HUD_H

#include <SFML/Graphics.hpp>

using namespace sf;

class HUD {
protected:
    Font font;
    Text text;
    RectangleShape background;

public:
    virtual ~HUD() = default;

    //M�thodes virtuelles pour dessiner et mettre � jour le HUD
    virtual void update(float deltaTime) = 0;
    virtual void draw(RenderWindow& window) = 0;
};

#endif
