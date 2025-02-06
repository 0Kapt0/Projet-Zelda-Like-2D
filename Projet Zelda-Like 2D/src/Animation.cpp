#include "../include/Animation.h"

Animation::Animation(int scene) : scene(scene), dialogue(1198, 200) {
    window.create(VideoMode(1200, 900), "Zeldouille", Style::None);


    if (!texture.loadFromFile("assets/enemy/boss/boss_for_md.png")) {
        cerr << "Erreur de chargement de la texture du ChaserEnemy !" << endl;
    }
    skip.setFont(font);
    skip.setCharacterSize(25);
    skip.setPosition(1070, 20);
    skip.setString("F pour skip");

    sprite.setTexture(texture);
    sprite.setPosition(280, 30);
    sprite.setScale(2.8f, 2.8f);

    dialogue.setTextSound("assets/enemy/boss/bossText.wav");
    dialogue.setPosition(1, 699);
    dialogue.setDialogueCharacterSize(200);
    dialogue.setDialogue({
        "Il y a longtemps, un monde paisible et prospere fut devaste par une force inconcevable 'le Void'. " ,
        "Ce mal sombre et insidieux a corrompu chaque recoin de la terre, plongeant les royaumes dans une ere de tenebres. " ,
        "Les paysages autrefois florissants sont desormais des ruines, et l'espoir s'effrite a chaque souffle du vent. ",
        "sauras-tu sauver ce monde du desespoir ?",
        });

    if (!font.loadFromFile("assets/fonts/American_Captain.ttf")) {
        cerr << "Erreur : Impossible de charger la police !" << endl;
    }

    speed = 1.0;

}

Animation::~Animation() {
    cout << "Animation est d�truit\n";
}

void Animation::update(float updateTime) {
    switch (scene) {
    case 1:
        firstScene();
        handlefirstSceneDialogue();
        break;
    case 2:
        secondScene();
        break;
    case 3:
        thirdScene();
        break;
    default:
        break;
    }
}

void Animation::updateDialogue(int scene) {
    if (scene == 1) {

    }
    else if (scene == 2) {

    }
    else if (scene == 3) {

    }
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

        updateDialogue(scene);
        update(updateTime);

        window.clear();
        draw(scene);
        window.display();
    }
}

void Animation::firstScene() {

}

void Animation::secondScene() {

}

void Animation::thirdScene() {

}

void Animation::handlefirstSceneDialogue() {
    if (!dialogue.isCurrentlyTyping() && !waitingForNextDialogue && !dialogue.isDialogueFinished()) {
        dialogueClock.restart();
        waitingForNextDialogue = true;
    }

    if (waitingForNextDialogue && dialogueClock.getElapsedTime().asSeconds() > 1.0f) {
        dialogue.advanceDialogue();
        waitingForNextDialogue = false;
    }

    dialogue.update();
}

void Animation::draw(int scene) {
    window.draw(skip);
    switch (scene) {
    case 1:
        window.draw(sprite);
        dialogue.draw(window);
        break;
    case 2:
        break;
    case 3:
        break;
    default:
        break;
    }
}