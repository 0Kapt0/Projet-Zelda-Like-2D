#ifndef ANIMATION_H
#define ANIMATION_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>
#include "ChaserEnemy.h"
#include "DialogueBox.h"
#include "Map.h"

using namespace sf;
using namespace std;

class Animation {
private:
    RenderWindow window;    
    Texture bossTexture;
    Sprite bossSprite;
    float speed;
    int scene;
    Clock clock;
    DialogueBox dialogue;
    Map map;
    Map corruptedMap;
    Clock dialogueClock;
    bool waitingForNextDialogue = false;
    Font font;
    Text skip;
    View view;
    float dialoguePosX;
    float dialoguePosY;
    float zoomFactor;
    Vector2f originalViewSize;
    // Fade j'y arrive pas lol
    //RectangleShape fadeOverlay;
    //float fadeAlpha = 0.0f;
    //bool isFading = false;
    //bool fadeIn = false;
    //bool isLoading = false;
public:
    Animation(int scene);
    ~Animation();

    void update(float updateTime);
    void Start();
    void firstScene();
    void secondScene();
    void thirdScene();
    void handleSceneDialogue();
    void draw(int scene);

};


#endif
