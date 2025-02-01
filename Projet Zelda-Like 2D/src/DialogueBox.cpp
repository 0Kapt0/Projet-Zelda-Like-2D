#include "../include/DialogueBox.h"
#include <iostream>

using namespace sf;
using namespace std;

DialogueBox::DialogueBox(float width, float height) : currentDialogue(0), charIndex(0), isTyping(false), dialogueFinished(true) {
    if (!font.loadFromFile("assets/fonts/American_Captain.ttf")) {
        cerr << "Error loading font\n";
    }

    if (!textBuffer.loadFromFile("assets/fonts/sounds/testText.wav")) {
        cerr << "Error loading text scroll sound\n";
    }
    textSound.setBuffer(textBuffer);
    textSound.setVolume(20);

    //Set up la dialogue box
    box.setSize(Vector2f(width, height));
    box.setFillColor(Color(0, 0, 0, 255));
    box.setOutlineThickness(1);
    box.setOutlineColor(Color::White);

    //Set up le text
    text.setFont(font);
    text.setCharacterSize(30);
    text.scale(0.25f, 0.25f);
    text.setFillColor(Color::White);
}

#include "../include/DialogueBox.h"
#include <iostream>

using namespace sf;
using namespace std;

//Définit le son du texte
void DialogueBox::setTextSound(const std::string& soundFile) {
    if (!textBuffer.loadFromFile(soundFile)) {
        cerr << "Error loading sound file: " << soundFile << "\n";
    }
    textSound.setBuffer(textBuffer);
}

//Définit la position du dialogue
void DialogueBox::setPosition(float x, float y) {
    box.setPosition(x, y);
    text.setPosition(x + 7, y + 2);
}

//Définit le dialogue actuel
void DialogueBox::setDialogue(const vector<string>& newDialogues) {
    dialogues = newDialogues;
    currentDialogue = 0;
    charIndex = 0;
    displayedText = "";
    isTyping = true;
    dialogueFinished = false;
    wrapText();
    text.setString("");
}

//Affiche progressivement le texte
void DialogueBox::update() {
    if (isTyping && clock.getElapsedTime().asMilliseconds() > 50) {
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

//Coupe le texte si trop long
void DialogueBox::wrapText() {
    text.setString("");
    vector<string> wrappedText;
    string currentLine;
    float scaleFactor = 0.3f;
    float boxWidth = (box.getSize().x - 4) / scaleFactor;

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

//Passe au texte suivant
void DialogueBox::advanceDialogue() {
    if (isTyping) {
        displayedText = dialogues[currentDialogue];
        charIndex = displayedText.length();
        text.setString(displayedText);
        isTyping = false;
        textSound.stop();
    }
    else if (clock.getElapsedTime().asSeconds() > 0.2f) {
        if (currentDialogue + 1 < dialogues.size()) {
            currentDialogue++;
            charIndex = 0;
            displayedText = "";
            isTyping = true;
            wrapText();
            clock.restart();
        }
        else {
            dialogueFinished = true;
        }
    }
}

//Arrête le son du texte
void DialogueBox::stopSound() {
    if (textSound.getStatus() == Sound::Playing) {
        textSound.stop();
    }
}

//Vérifie si le dialogue est fini
bool DialogueBox::isDialogueFinished() const {
    return dialogueFinished;
}

//Guetter de l'index du dialogue actuel
int DialogueBox::getCurrentDialogueIndex() const {
    return currentDialogue;
}

//Guetter de la taille du dialogue
int DialogueBox::getDialogueSize() const {
    return dialogues.size();
}

//Vérifie si le texte défile
bool DialogueBox::isCurrentlyTyping() const {
    return isTyping;
}

//Affiche la boîte de dialogue
void DialogueBox::draw(RenderWindow& window) {
    if (!dialogueFinished) {
        window.draw(box);
        window.draw(text);
    }
}
