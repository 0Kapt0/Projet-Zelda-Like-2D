#include "../include/AlliedEntity.h"

using namespace sf;
using namespace std;

AlliedEntity::AlliedEntity(const string& _name, float x, float y)
    : name(_name), isTalking(false) {
    dialogueBox = make_unique<DialogueBox>(100.0f, 30.0f);
}

void AlliedEntity::interact() {
}

//Fonction pour démarrer un dialogue avec une liste de phrases
void AlliedEntity::startDialogue(const vector<string>& dialogues) {
    if (dialogueBox) {
        dialogueBox->setDialogue(dialogues);
        isTalking = true;
    }
}

void AlliedEntity::advanceDialogue() {
    if (isTalking && dialogueBox) {
        dialogueBox->advanceDialogue();

        //Ne pas fermer immédiatement, attendre une confirmation
        if (dialogueBox->isDialogueFinished()) {
            isTalking = false;
        }
    }
}

//Met à jour le dialogue (permet de passer au texte suivant)
void AlliedEntity::updateDialogue() {
    if (isTalking && dialogueBox) {
        dialogueBox->update();
        if (!dialogueBox->isCurrentlyTyping() &&
            dialogueBox->getCurrentDialogueIndex() >= dialogueBox->getDialogueSize() - 1) {
            isTalking = false;
        }
    }
}

// Vérifie la collision avec le joueur et déclenche le dialogue si nécessaire
void AlliedEntity::checkCollisionWithPlayer(Player& player) {
    if (shape.getGlobalBounds().intersects(player.getShape().getGlobalBounds())) {
        if (Keyboard::isKeyPressed(Keyboard::E) && !isTalking) {
            interact();
        }
    }
}

// Met à jour la boîte de dialogue et la position
void AlliedEntity::update(float deltaTime, const RenderWindow& window, const Vector2f& playerPosition, Map& map) {
    animate(deltaTime);

    if (dialogueBox) {
        dialogueBox->setPosition(shape.getPosition().x - 35, shape.getPosition().y - 35);
    }

    if (isTalking) {
        updateDialogue();
    }
}

bool AlliedEntity::isDialogueActive() const {
    return isTalking;
}

void AlliedEntity::setDialogueActive(bool active) {
    isTalking = active;
    if (!active && dialogueBox) {
        dialogueBox->stopSound();
    }
}

bool AlliedEntity::isDialogueFinished() const {
    return dialogueBox ? dialogueBox->isDialogueFinished() : true;
}

void AlliedEntity::draw(RenderWindow& window) {
    window.draw(shape);
    if (isTalking && dialogueBox && !dialogueBox->isDialogueFinished()) {
        dialogueBox->draw(window);
    }
}
