#include "../include/AlliedEntity.h"

using namespace sf;
using namespace std;

AlliedEntity::AlliedEntity(const string& _name, float x, float y)
    : name(_name), dialogueBox(100, 30), isTalking(false) {
    shape.setPosition(x, y);
    dialogueBox.setPosition(x, y);
}

void AlliedEntity::interact() {
}

//Fonction pour démarrer un dialogue avec une liste de phrases
void AlliedEntity::startDialogue(const vector<string>& dialogues) {
    dialogueBox.setDialogue(dialogues);
    isTalking = true;
}

void AlliedEntity::advanceDialogue() {
    if (isTalking) {
        dialogueBox.advanceDialogue();

        //Si on atteint la fin du dialogue, on arrête de parler
        if (!dialogueBox.isCurrentlyTyping() &&
            dialogueBox.getCurrentDialogueIndex() >= dialogueBox.getDialogueSize() - 1) {
            isTalking = false;
        }
    }
}

//Met à jour le dialogue (permet de passer au texte suivant)
void AlliedEntity::updateDialogue() {
    if (isTalking) {
        dialogueBox.update();
        if (!dialogueBox.isCurrentlyTyping() && dialogueBox.getCurrentDialogueIndex() >= dialogueBox.getDialogueSize() - 1) {
            isTalking = false;
        }
    }
}

//Vérifie la collision avec le joueur et déclenche le dialogue si nécessaire
void AlliedEntity::checkCollisionWithPlayer(Player& player) {
    if (shape.getGlobalBounds().intersects(player.getShape().getGlobalBounds())) {
        interact();
    }
}

//Met à jour la boîte de dialogue et la position
void AlliedEntity::update(float deltaTime, const RenderWindow& window, const Vector2f& playerPosition, Map& map) {
    animate(deltaTime);
    dialogueBox.setPosition(shape.getPosition().x - 35, shape.getPosition().y - 35);
    if (isTalking) {
        updateDialogue();
    }
}

//Affichage de l'entité + dialogue si nécessaire
void AlliedEntity::draw(RenderWindow& window) {
    window.draw(shape);
    if (isTalking) {
        dialogueBox.draw(window);
    }
}
