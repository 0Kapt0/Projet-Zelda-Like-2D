#include "../include/Merchant.h"
#include <iostream>

using namespace sf;
using namespace std;

//Texture statique partagée entre tous les marchands
Texture Merchant::merchantTex;

//Charge la texture du marchand
void Merchant::loadMerchantTexture(const string& texturePath) {
    if (!merchantTex.loadFromFile(texturePath)) {
        cerr << "Merchant texture not loaded" << endl;
    }
    else {
        cout << "Merchant texture loaded" << endl;
    }
}

//Constructeur du marchand
Merchant::Merchant(float x, float y)
    : AlliedEntity("Marchand", x, y) {
    shape.setTexture(&merchantTex);
    shape.setSize(Vector2f(32.0f, 32.0f));
    shape.setPosition(x, y);
}

//Met à jour le marchand
void Merchant::update(float deltaTime, const RenderWindow& window, const Vector2f& playerPosition, Map& map) {
    animate(deltaTime);

    if (dialogueBox) {
        dialogueBox->setPosition(shape.getPosition().x - 35, shape.getPosition().y - 35);
    }

    //Distance entre le joueur et le marchand
    float distance = sqrt(pow(playerPosition.x - shape.getPosition().x, 2) +
        pow(playerPosition.y - shape.getPosition().y, 2));

    //Distance max avant fermeture du dialogue
    float maxDistance = 96.0f;

    if (distance > maxDistance) {
        setDialogueActive(false);
    }

    if (isDialogueActive()) {
        updateDialogue();
    }
}

//Interaction avec le joueur
void Merchant::interact() {
    if (!isDialogueActive()) {
        dialogueBox->setTextSound("assets/NPC/sounds/merchantText.wav");

        startDialogue({
            "Bonjour aventurier !",
            "Bienvenue je vais t'apprendre a utilise des potion et a te deplacer.",
            "Tu vois la potion la haut utilise Z Q S D pour te deplacer.",
            "Maintenant appuie sur H pour utiliser ta potion.",
            "Fais attention aventurier ta potion met 10s a se recharger.",
            "Aventurier attends !!",
            "Utilise espace pour attaquer et shift pour dasher.",
            "Bonne chance aventurier que la lumiere sois avec toi.",
            "."
            });

        setDialogueActive(true);
    }
    else {
        advanceDialogue();
    }
}
