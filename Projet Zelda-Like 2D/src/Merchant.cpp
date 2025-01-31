#include "../include/Merchant.h"
#include <iostream>

using namespace sf;
using namespace std;

Merchant::Merchant(float x, float y)
    : AlliedEntity("Marchand", x, y) {
    if (!merchantTex.loadFromFile("assets/NPC/merchant.png")) {
        cerr << "Erreur lors du chargement de la texture du marchand !" << endl;
    }
    else {
        cout << "Texture du marchand chargée avec succes !" << endl;
    }

    shape.setTexture(&merchantTex);
    shape.setSize(Vector2f(32.0f, 32.0f));
    shape.setPosition(x, y);
}

void Merchant::interact() {
    startDialogue({
        "Bonjour aventurier !",
        "Bienvenue dans ma boutique.",
        "J'ai des objets rares a vendre, jette y un oeil !"
        });
}
