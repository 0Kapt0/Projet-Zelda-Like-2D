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

//Fonction pour d�marrer un dialogue avec une liste de phrases
void AlliedEntity::startDialogue(const vector<string>& dialogues) {
    dialogueBox.setDialogue(dialogues);
    isTalking = true;
}

void AlliedEntity::advanceDialogue() {
    if (isTalking) {
        dialogueBox.advanceDialogue();

        //Si on atteint la fin du dialogue, on arr�te de parler
        if (!dialogueBox.isCurrentlyTyping() &&
            dialogueBox.getCurrentDialogueIndex() >= dialogueBox.getDialogueSize() - 1) {
            isTalking = false;
        }
    }
}

//Met � jour le dialogue (permet de passer au texte suivant)
void AlliedEntity::updateDialogue() {
    if (isTalking) {
        dialogueBox.update();
        if (!dialogueBox.isCurrentlyTyping() && dialogueBox.getCurrentDialogueIndex() >= dialogueBox.getDialogueSize() - 1) {
            isTalking = false;
        }
    }
}

//V�rifie la collision avec le joueur et d�clenche le dialogue si n�cessaire
void AlliedEntity::checkCollisionWithPlayer(Player& player) {
    if (shape.getGlobalBounds().intersects(player.getShape().getGlobalBounds())) {
        interact();
    }
}

//Met � jour la bo�te de dialogue et la position
void AlliedEntity::update(float deltaTime, const RenderWindow& window, const Vector2f& playerPosition, Map& map) {
    animate(deltaTime);
    dialogueBox.setPosition(shape.getPosition().x - 35, shape.getPosition().y - 35);
    if (isTalking) {
        updateDialogue();
    }
}

//Affichage de l'entit� + dialogue si n�cessaire
void AlliedEntity::draw(RenderWindow& window) {
    window.draw(shape);
    if (isTalking) {
        dialogueBox.draw(window);
    }
}
