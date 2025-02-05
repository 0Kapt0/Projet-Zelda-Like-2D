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
    //Éléments graphiques
    Font font;
    Text text;
    RectangleShape box;

    //Gestion du texte
    vector<string> dialogues;
    string displayedText;
    size_t currentDialogue;
    size_t charIndex;
    bool isTyping;
    bool dialogueFinished;

    //Gestion du son
    SoundBuffer textBuffer;
    Sound textSound;

    //Chrono
    Clock clock;

public:
    //Constructeur
    DialogueBox(float width, float height);

    //set up
    void setTextSound(const string& soundFile);
    void setDialogue(const vector<string>& newDialogues);
    void setPosition(float x, float y);
    void setDialogueCharacterSize(int size);

    //Gestion du texte
    void wrapText();
    void update();
    void advanceDialogue();
    void stopSound();

    //Affichage
    void draw(RenderWindow& window);

    //Guetters
    int getCurrentDialogueIndex() const;
    int getDialogueSize() const;
    bool isCurrentlyTyping() const;
    bool isDialogueFinished() const;
    string getCurrentText() const;
};

#endif
