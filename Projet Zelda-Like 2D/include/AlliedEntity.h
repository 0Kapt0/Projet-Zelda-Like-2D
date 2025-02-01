#ifndef ALLIEDENTITY_H
#define ALLIEDENTITY_H

#include "Entity.h"
#include "Player.h"
#include "DialogueBox.h"
#include <memory>

using namespace sf;
using namespace std;

class AlliedEntity : public Entity {
protected:
    string name;
    unique_ptr<DialogueBox> dialogueBox;
    bool isTalking;

public:
    AlliedEntity(const string& _name, float x, float y);
    virtual ~AlliedEntity() {}

    void checkCollisionWithPlayer(Player& player);
    virtual void interact();

    void startDialogue(const vector<string>& dialogues);
    void advanceDialogue();
    void updateDialogue();

    bool isDialogueActive() const;
    void setDialogueActive(bool active);
    bool isDialogueFinished() const;

    void update(float deltaTime, const RenderWindow& window, const Vector2f& playerPosition, Map& map) override;
    void draw(RenderWindow& window) override;
};

#endif
