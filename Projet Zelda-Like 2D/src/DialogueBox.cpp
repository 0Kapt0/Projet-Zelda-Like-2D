#include "../include/DialogueBox.h"
#include <iostream>

using namespace sf;
using namespace std;

DialogueBox::DialogueBox(float width, float height) : currentDialogue(0), charIndex(0), isTyping(false) {
    if (!font.loadFromFile("assets/fonts/American_Captain.ttf")) {
        cerr << "Error loading font\n";

    }
    if (!font.loadFromFile("assets/fonts/American_Captain.ttf")) {
        cerr << "Error loading font\n";
    }

    if (!textBuffer.loadFromFile("assets/fonts/sounds/testText.wav")) {
        cerr << "Error loading text scroll sound\n";
    }
    textSound.setBuffer(textBuffer);
    textSound.setVolume(20);

    //Set up dialogue box
    box.setSize(Vector2f(width, height));
    box.setFillColor(Color(0, 0, 0, 255));
    box.setOutlineThickness(1);
    box.setOutlineColor(Color::White);

    //Set up text
    text.setFont(font);
    text.setCharacterSize(30);
    text.scale(0.25f, 0.25f);
    text.setFillColor(Color::White);
}

void DialogueBox::setPosition(float x, float y) {
    box.setPosition(x, y);
    text.setPosition(x + 7, y + 2);
}

void DialogueBox::setDialogue(const vector<string>& newDialogues) {
    dialogues = newDialogues;
    currentDialogue = 0;
    charIndex = 0;
    displayedText = "";
    isTyping = true;

    wrapText();
    text.setString("");
}

void DialogueBox::update() {
    if (isTyping && clock.getElapsedTime().asMilliseconds() > 50) { //Contrôle la vitesse
        if (charIndex < dialogues[currentDialogue].length()) {
            displayedText += dialogues[currentDialogue][charIndex++];
            text.setString(displayedText);
            if (textSound.getStatus() != Sound::Playing) {
                textSound.play();
            }
            clock.restart();
        }
        else {
            isTyping = false;
            textSound.stop();
        }
    }
}

void DialogueBox::wrapText() {
    text.setString("");
    vector<string> wrappedText;
    string currentLine;
    float scaleFactor = 0.3f;
    float boxWidth = (box.getSize().x - 14) / scaleFactor;

    istringstream words(dialogues[currentDialogue]);
    string word;

    while (words >> word) {
        string testLine = currentLine + (currentLine.empty() ? "" : " ") + word;
        text.setString(testLine);

        if (text.getLocalBounds().width > boxWidth) {
            wrappedText.push_back(currentLine);
            currentLine = word;
        }
        else {
            currentLine = testLine;
        }
    }

    if (!currentLine.empty()) {
        wrappedText.push_back(currentLine);
    }

    //Vérification du nombre de lignes affichées
    if (wrappedText.size() > 3) {
        wrappedText.resize(3);
    }

    dialogues[currentDialogue] = wrappedText[0];
    for (size_t i = 1; i < wrappedText.size(); ++i) {
        dialogues[currentDialogue] += "\n" + wrappedText[i];
    }

    displayedText = "";
    charIndex = 0;
    isTyping = true;
}




void DialogueBox::advanceDialogue() {
    if (isTyping) {
        displayedText = dialogues[currentDialogue];
        charIndex = displayedText.length();
        text.setString(displayedText);
        isTyping = false;
        textSound.stop();
    }
    else {
        if (currentDialogue + 1 < dialogues.size()) {
            currentDialogue++;
            charIndex = 0;
            displayedText = "";
            isTyping = true;

            wrapText();
        }
    }
}

int DialogueBox::getCurrentDialogueIndex() const {
    return currentDialogue;
}

int DialogueBox::getDialogueSize() const {
    return dialogues.size();
}

bool DialogueBox::isCurrentlyTyping() const {
    return isTyping;
}

void DialogueBox::draw(RenderWindow& window) {
    window.draw(box);
    window.draw(text);
}
