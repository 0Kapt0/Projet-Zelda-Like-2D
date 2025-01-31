#ifndef DIALOGUEBOX_H
#define DIALOGUEBOX_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <string>
#include <sstream>

using namespace sf;
using namespace std;

class DialogueBox {
private:
    Font font;
    Text text;
    RectangleShape box;
    Clock clock;

    vector<string> dialogues;
    size_t currentDialogue;
    size_t charIndex;
    string displayedText;
    bool isTyping;

    SoundBuffer textBuffer;
    Sound textSound;
public:
    DialogueBox(float width, float height);
    void setDialogue(const vector<string>& newDialogues);
    void setPosition(float x, float y);
    void wrapText();
    void update();
    void advanceDialogue();
    void draw(RenderWindow& window);
};

#endif
