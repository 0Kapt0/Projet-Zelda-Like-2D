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

        //Ne pas fermer immédiatement, attendre une confirmation
        if (dialogueBox.isDialogueFinished()) {
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
        if (Keyboard::isKeyPressed(Keyboard::E) && !isTalking) {
            interact();
        }
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

bool AlliedEntity::isDialogueActive() const {
    return isTalking;
}

void AlliedEntity::setDialogueActive(bool active) {
    isTalking = active;
    if (!active) {
        dialogueBox.stopSound();
    }
}

bool AlliedEntity::isDialogueFinished() const {
    return dialogueBox.isDialogueFinished();
}

void AlliedEntity::draw(RenderWindow& window) {
    window.draw(shape);
    if (isTalking && !dialogueBox.isDialogueFinished()) {
        dialogueBox.draw(window);
    }
}
