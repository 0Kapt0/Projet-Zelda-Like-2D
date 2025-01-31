#include "../include/DialogueBox.h"
#include <iostream>

DialogueBox::DialogueBox(float width, float height, int posx, int posy) : currentDialogue(0), charIndex(0), isTyping(false) {
    if (!font.loadFromFile("assets/fonts/American_Captain.ttf")) { // Ensure you have a valid font file
        std::cerr << "Error loading font\n";
    }

    // Set up dialogue box
    box.setSize(sf::Vector2f(width, height));
    box.setFillColor(sf::Color(0, 0, 0, 200));
    box.setOutlineThickness(2);
    box.setOutlineColor(sf::Color::White);
    box.setPosition(posx, posy);

    // Set up text
    text.setFont(font);
    text.setCharacterSize(30);
    text.scale(0.3f, 0.3f);
    text.setFillColor(sf::Color::White);
    text.setPosition(posx + 10, posy + 10);
}

void DialogueBox::setPosition(float x, float y) {
    box.setPosition(x, y);  // Set the position of the box
    text.setPosition(x + 15, y + 15); // Position the text with padding
}

void DialogueBox::setDialogue(const std::vector<std::string>& newDialogues) {
    dialogues = newDialogues;
    currentDialogue = 0;
    charIndex = 0;
    displayedText = "";
    isTyping = true;
}

void DialogueBox::update() {
    if (isTyping && clock.getElapsedTime().asMilliseconds() > 50) { // Adjust speed here
        if (charIndex < dialogues[currentDialogue].length()) {
            displayedText += dialogues[currentDialogue][charIndex++];
            text.setString(displayedText);
            clock.restart();
        }
        else {
            isTyping = false;
        }
    }
}

void DialogueBox::advanceDialogue() {
    if (isTyping) {
        displayedText = dialogues[currentDialogue]; // Instantly complete text
        charIndex = displayedText.length();
        text.setString(displayedText);
        isTyping = false;
    }
    else {
        if (currentDialogue + 1 < dialogues.size()) {
            currentDialogue++;
            charIndex = 0;
            displayedText = "";
            isTyping = true;
        }
    }
}

void DialogueBox::draw(sf::RenderWindow& window) {
    window.draw(box);
    window.draw(text);
}
