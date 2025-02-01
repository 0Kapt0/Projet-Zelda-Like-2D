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

void Merchant::update(float deltaTime, const RenderWindow& window, const Vector2f& playerPosition, Map& map) {
    animate(deltaTime);
    dialogueBox.setPosition(shape.getPosition().x - 35, shape.getPosition().y - 35);

    //Calcule la distance entre le joueur et le marchand
    float distance = sqrt(pow(playerPosition.x - shape.getPosition().x, 2) +
        pow(playerPosition.y - shape.getPosition().y, 2));

    //distance de la dialoge box
    float maxDistance = 96.0f;

    if (distance > maxDistance) {
        setDialogueActive(false);
    }

    if (isDialogueActive()) {
        updateDialogue();
    }
}


void Merchant::interact() {
    if (!isDialogueActive()) {
        dialogueBox.setTextSound("assets/NPC/sounds/merchantText.wav");

        startDialogue({
            "Bonjour aventurier !",
            "Bienvenue dans ma boutique.",
            "J'ai des objets rares a vendre, jette-y un oeil !",
            "Fais ton choix aventurier !",
            "."
            });
        setDialogueActive(true);
    }
}