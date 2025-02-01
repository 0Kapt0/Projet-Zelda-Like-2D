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

//D�marre un dialogue
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

        // Ferme le dialogue si termin�
        if (dialogueBox->isDialogueFinished()) {
            isTalking = false;
        }
    }
}

//Met � jour le texte affich�
void AlliedEntity::updateDialogue() {
    if (isTalking && dialogueBox) {
        dialogueBox->update();
        if (!dialogueBox->isCurrentlyTyping() &&
            dialogueBox->getCurrentDialogueIndex() >= dialogueBox->getDialogueSize() - 1) {
            isTalking = false;
        }
    }
}

//V�rifie la collision avec le joueur
void AlliedEntity::checkCollisionWithPlayer(Player& player) {
    if (shape.getGlobalBounds().intersects(player.getShape().getGlobalBounds())) {
        if (Keyboard::isKeyPressed(Keyboard::E) && !isTalking) {
            interact();
        }
    }
}

//Met � jour l'entit� et son dialogue
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

//Active ou d�sactive un dialogue
void AlliedEntity::setDialogueActive(bool active) {
    isTalking = active;
    if (!active && dialogueBox) {
        dialogueBox->stopSound();
    }
}

//V�rifie si le dialogue est termin�
bool AlliedEntity::isDialogueFinished() const {
    return dialogueBox ? dialogueBox->isDialogueFinished() : true;
}

//Dessine l'entit� et sa bo�te de dialogue
void AlliedEntity::draw(RenderWindow& window) {
    window.draw(shape);
    if (isTalking && dialogueBox && !dialogueBox->isDialogueFinished()) {
        dialogueBox->draw(window);
    }
}
