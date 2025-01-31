#ifndef DIALOGUEBOX_H
#define DIALOGUEBOX_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class DialogueBox {
private:
    sf::RectangleShape box;
    sf::Text text;
    sf::Font font;
    std::vector<std::string> dialogues;
    size_t currentDialogue;
    std::string displayedText;
    size_t charIndex;
    sf::Clock clock;
    bool isTyping;

public:
    DialogueBox(float width, float height, int posx, int posy);
    void setDialogue(const std::vector<std::string>& newDialogues);
    void setPosition(float x, float y);
    void update();
    void advanceDialogue();
    void draw(sf::RenderWindow& window);
};

#endif
