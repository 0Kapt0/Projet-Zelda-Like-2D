#include "../include/AlliedEntity.h"

using namespace sf;
using namespace std;

//Constructeur
AlliedEntity::AlliedEntity(const string& _name, float x, float y)
    : name(_name), isTalking(false) {
    dialogueBox = make_unique<DialogueBox>(100.0f, 30.0f);
}

//Interaction avec le joueur
void AlliedEntity::interact() {
}

//Démarre un dialogue
void AlliedEntity::startDialogue(const vector<string>& dialogues) {
    if (dialogueBox) {
        dialogueBox->setDialogue(dialogues);
        isTalking = true;
    }
}

//Passe au dialogue suivant
void AlliedEntity::advanceDialogue() {
    if (isTalking && dialogueBox) {
        dialogueBox->advanceDialogue();

        // Ferme le dialogue si terminé
        if (dialogueBox->isDialogueFinished()) {
            isTalking = false;
        }
    }
}

//Met à jour le texte affiché
void AlliedEntity::updateDialogue() {
    if (isTalking && dialogueBox) {
        dialogueBox->update();
        if (!dialogueBox->isCurrentlyTyping() &&
            dialogueBox->getCurrentDialogueIndex() >= dialogueBox->getDialogueSize() - 1) {
            isTalking = false;
        }
    }
}

//Vérifie la collision avec le joueur
void AlliedEntity::checkCollisionWithPlayer(Player& player) {
    if (shape.getGlobalBounds().intersects(player.getShape().getGlobalBounds())) {
        if (Keyboard::isKeyPressed(Keyboard::E) && !isTalking) {
            interact();
        }
    }
}

//Met à jour l'entité et son dialogue
void AlliedEntity::update(float deltaTime, const RenderWindow& window, const Vector2f& playerPosition, Map& map) {
    animate(deltaTime);

    if (dialogueBox) {
        dialogueBox->setPosition(shape.getPosition().x - 35, shape.getPosition().y - 35);
    }

    if (isTalking) {
        updateDialogue();
    }
}

//Retourne si un dialogue est actif
bool AlliedEntity::isDialogueActive() const {
    return isTalking;
}

//Active ou désactive un dialogue
void AlliedEntity::setDialogueActive(bool active) {
    isTalking = active;
    if (!active && dialogueBox) {
        dialogueBox->stopSound();
    }
}

//Vérifie si le dialogue est terminé
bool AlliedEntity::isDialogueFinished() const {
    return dialogueBox ? dialogueBox->isDialogueFinished() : true;
}

//Dessine l'entité et sa boîte de dialogue
void AlliedEntity::draw(RenderWindow& window) {
    window.draw(shape);
    if (isTalking && dialogueBox && !dialogueBox->isDialogueFinished()) {
        dialogueBox->draw(window);
    }
}
