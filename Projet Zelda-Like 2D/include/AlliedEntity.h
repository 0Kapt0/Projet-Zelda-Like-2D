#ifndef ALLIEDENTITY_H
#define ALLIEDENTITY_H

#include "Entity.h"
#include "Player.h"
#include "DialogueBox.h"

using namespace sf;
using namespace std;

class AlliedEntity : public Entity {
protected:
    string name;
    DialogueBox dialogueBox;
    bool isTalking;

public:
    AlliedEntity(const string& _name, float x, float y);
    virtual ~AlliedEntity() {}

    void checkCollisionWithPlayer(Player& player);
    virtual void interact();

    void startDialogue(const vector<string>& dialogues);
    void advanceDialogue();
    void updateDialogue();

    void update(float deltaTime, const RenderWindow& window, const Vector2f& playerPosition, Map& map) override;
    void draw(RenderWindow& window) override;
};

#endif
