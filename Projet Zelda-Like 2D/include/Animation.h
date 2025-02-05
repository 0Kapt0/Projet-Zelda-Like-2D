#ifndef ANIMATION_H
#define ANIMATION_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>
#include "ChaserEnemy.h"
#include "DialogueBox.h"

using namespace sf;
using namespace std;

class Animation {
private:
    RenderWindow window;    
    Texture texture;
    Sprite sprite;
    float speed;
    int scene;
    Clock clock;
    DialogueBox dialogue;
    Clock dialogueClock;
    bool waitingForNextDialogue = false;
    Font font;
public:
    Animation(int scene);
    ~Animation();

    void update(float updateTime);
    void updateDialogue(int scene);
    void Start();
    void firstScene();
    void secondScene();
    void thirdScene();
    void handlefirstSceneDialogue();
    void draw(int scene);

};


#endif
