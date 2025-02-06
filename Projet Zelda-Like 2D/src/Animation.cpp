#include "../include/Animation.h"

Animation::Animation(int scene) : 
    scene(scene),
    dialogue(498, 100),
    map("assets/maps/lobby.txt", "assets/tilesets/Tileset_Grass.png", "assets/tilesets/items.png", 32, { 65 }, { 72, 73, 80, 81, 88, 89 }),
    corruptedMap("assets/maps/corruptedLobby.txt", "assets/tilesets/Tileset_Grass.png", "assets/tilesets/items.png", 32, { 65 }, { 72, 73, 80, 81, 88, 89 }),
    view(sf::FloatRect(0, 0, 500, 500)),
    Joseph(Vector2f(5000, 5000)) {

    window.create(VideoMode(1200, 900), "Zeldouille", Style::None);
    window.setView(view);

    if (!bossTexture.loadFromFile("assets/enemy/boss/boss_for_md.png")) {
        cerr << "Erreur de chargement de la texture du boss !" << endl;
    }
    if (!playerTexture.loadFromFile("assets/player/player_for_md.png")) {
        cerr << "Erreur de chargement de la texture du player !\n";
    }

    Joseph.setFillColor(Color(0, 0, 0, 175));

    skip.setFont(font);
    skip.setCharacterSize(15);
    skip.setPosition(1070, 20);
    skip.setString("F pour skip");

    bossSprite.setTexture(bossTexture);
    bossSprite.setPosition(280, 30);
    bossSprite.setScale(2.8f, 2.8f);

    playerSprite.setTexture(playerTexture);
    playerSprite.setPosition(475, 300);
    playerSprite.setScale(5, 5);

    zoomFactor = 1.0f;
    originalViewSize = view.getSize();

    //fadeOverlay.setSize(sf::Vector2f(5000, 5000));
    //fadeOverlay.setPosition(0, 0);
    //fadeOverlay.setFillColor(sf::Color(0, 0, 0, 0));

    dialoguePosX = 1;
    dialoguePosY = 399;
    dialogue.setPosition(dialoguePosX, dialoguePosY);
    dialogue.setDialogueCharacterSize(100);
    dialogue.setDialogue({
        "Il y a longtemps, un monde paisible et prospere fut devaste par une force inconcevable 'le Void'. " ,
        "Ce mal sombre et insidieux a corrompu chaque recoin de la terre, plongeant les royaumes dans une ere de tenebres. " ,
        "Mais une prophetie ancienne parlait d'un heros qui, un jour, se dresserait contre lui et sauverait ce monde englouti par les tenebres...  "
        });

    if (!font.loadFromFile("assets/fonts/American_Captain.ttf")) {
        cerr << "Erreur : Impossible de charger la police !" << endl;
    }

    speed = 1.0;

}

Animation::~Animation() {
    cout << "Animation est détruit\n";
}

void Animation::update(float updateTime) {
    switch (scene) {
    case 1:
        firstScene();
        handleSceneDialogue();
        break;
    case 2:
        secondScene();
        handleSceneDialogue();
        break;
    case 3:
        thirdScene();
        handleSceneDialogue();
        break;
    default:
        break;
    }
    window.setView(view);
}

void Animation::Start() {
    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }
            if (Keyboard::isKeyPressed(Keyboard::F) || (dialogue.isDialogueFinished())) {
                window.close();
            }
        }

        float updateTime = clock.restart().asSeconds();

        update(updateTime);

        window.clear();
        draw(scene);
        window.display();
    }
}

void Animation::firstScene() {
    view.move(0.01f, 0.01f);
    dialogue.setPosition(dialoguePosX += 0.01f, dialoguePosY += 0.01f);
}

void Animation::secondScene() {
    view.move(0.01f, 0.01f);
    dialogue.setPosition(dialoguePosX += 0.01f, dialoguePosY += 0.01f);
    if (zoomFactor < 1.5f) {
        zoomFactor *= 1.000025f;
        view.setSize(originalViewSize * zoomFactor);
    }
}

void Animation::thirdScene() {
    view.setCenter(550, 450);
    dialogue.setPosition(301, 599);
}

void Animation::handleSceneDialogue() {
    if (!dialogue.isCurrentlyTyping() && !waitingForNextDialogue && !dialogue.isDialogueFinished()) {
        dialogueClock.restart();
        waitingForNextDialogue = true;
    }

    if (waitingForNextDialogue && dialogueClock.getElapsedTime().asSeconds() > 3.0f) {
        dialogue.advanceDialogue();
        waitingForNextDialogue = false;
        scene += 1;
        view.setCenter(250, 250);
        dialoguePosX = 1;
        dialoguePosY = 399;
        dialogue.setPosition(dialoguePosX, dialoguePosY);
        view.reset(sf::FloatRect(0, 0, 500, 500));

    }

    dialogue.update();
}

void Animation::draw(int scene) {
    window.draw(skip);
    switch (scene) {
    case 1:
        map.draw(window);
        break;
    case 2:
        corruptedMap.draw(window);
        window.draw(bossSprite);
        window.draw(Joseph);
        break;
    case 3:
        map.draw(window);
        window.draw(playerSprite);
        break;
    default:
        break;
    }
    dialogue.draw(window);
}