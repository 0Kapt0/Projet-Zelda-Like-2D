﻿#include "../include/GameState.h"
#include "../include/Merchant.h"

using namespace sf;
using namespace std;

// --- Constructeur ---
GameState::GameState(RenderWindow& window, Player& player, int gameState)
    : State(window),
    player(player),
    merchant(450, 190),
    map("assets/maps/lobby.txt", "assets/tilesets/Tileset_Grass.png", "assets/tilesets/items.png", 32, { 65 }, { 72, 73, 80, 81, 88, 89 }),
    gameState(gameState),
    lobby(true),
    hud(player) {

    Merchant::loadMerchantTexture("assets/NPC/merchant.png");

    if (!ambientSound.openFromFile("assets/music/dungeon_ambient_1.ogg")) {
        cerr << "Erreur de chargement du son d'ambiance !\n";
    }
    else {
        ambientSound.setLoop(true);
        ambientSound.setVolume(5);
        ambientSound.play();
    }

    if (!potionTexture.loadFromFile("assets/HUD/health_potion.png")) {
        cerr << "Erreur de chargement de la texture des potions\n";
    }

    if (!chestClosedTexture.loadFromFile("assets/chests/chest_closed.png")) {
        cerr << "Erreur de chargement de la texture des potions\n";
    }

    if (!chestOpenTexture.loadFromFile("assets/chests/chest_open.png")) {
        cerr << "Erreur de chargement de la texture des potions\n";
    }

    //Génère les potions sur la carte
    for (const auto& pos : map.getPotionPositions()) {
        Sprite potionSprite;
        potionSprite.setTexture(potionTexture);
        potionSprite.setPosition(pos);
        potionSprite.setScale(1.f, 1.f);
        potions.push_back(potionSprite);
    }

    //generate chests
    for (const auto& pos : map.getChestPositions()) {
        Sprite chestSprite;
        chestSprite.setTexture(chestClosedTexture);
        chestSprite.setPosition(pos);
        chestSprite.setScale(1.f, 1.f);
        chest_closed.push_back(chestSprite);
    }

    //Fade chargement
    fadeOverlay.setSize(Vector2f(window.getSize().x, window.getSize().y));
    fadeOverlay.setFillColor(Color(0, 0, 0, 0));

    //Initialise le joueur et les entités
    player.setPosition(map.getPlayerStartPosition());
    spawnEnemies();
    spawnNPCs();
}

GameState::~GameState() {
    ambientSound.stop();
}

void GameState::loadFromFile(Texture texture, std::string path) {
    if (!texture.loadFromFile(path)) {
        throw std::runtime_error("Erreur : Mauvais chemin pour la texture !");
    }
}
// --- Génère les ennemis ---
void GameState::spawnEnemies() {
    boss = make_unique<BossEnemy>(map.getBossPosition().x, map.getBossPosition().y, 200.0f, player);

    for (const auto& pos : map.getChaserEnemyPositions()) {
        chaserEnemies.push_back(make_unique<ChaserEnemy>(pos.x-80, pos.y, 50.0f, 100, 150.0f, player));
    }
    for (const auto& pos : map.getPatternEnemyPositions()) {
        patternEnemies.push_back(make_unique<PatternEnemy>(pos.x, pos.y+8, 10.0f, 100.f, 0.f, player));
    }
}

// --- Génère les NPCs ---
void GameState::spawnNPCs() {
    for (const auto& pos : map.getNpcPositions()) {
        npcs.emplace_back(make_unique<Merchant>(pos.x, pos.y));
    }
}

// --- Gère les entrées clavier ---
void GameState::handleInput() {
    /*if (isLoading) return;*/
    if (Keyboard::isKeyPressed(Keyboard::E) && textCD.getElapsedTime().asSeconds() > 0.2f) {
        for (auto& npc : npcs) {
            if (npc->isDialogueActive()) {
                npc->interact();
            }
        }

        if (merchant.isDialogueActive()) {
            merchant.interact();
        }

        textCD.restart();
    }
}

// --- Met à jour l'état du jeu ---
void GameState::update(float deltaTime) {
    player.update(deltaTime, window, player.getPosition(), map);
    hud.update(deltaTime);
    updateFade(deltaTime);

    int itemID = map.getItemAt(player.getPosition());

    if (itemID == -1) {
        cerr << "Erreur : Item inexistant à cette position !" << endl;
        return;
    }

    if (itemID == 63) { //TP vers le donjon
        changeMap("assets/maps/room_2.txt", false);
    }

    if (itemID == 60) { //Retour au lobby
        lobby = true;
        changeMap("assets/maps/lobby.txt", true);
    }

    if (itemID == 61) { //TP vers la map principale
        lobby = false;
        changeMap("assets/maps/room_1.txt", false);
    }

    if (itemID == 51) { //TP vers la map principale (alternatif)
        changeMap("assets/maps/room_1.txt", true);
    }

    for (auto& enemy : chaserEnemies) {
        enemy->update(deltaTime, window, player.getPosition(), map);
    }

    for (auto& enemy : patternEnemies) {
        enemy->update(deltaTime, window, player.getPosition(), map);
    }

    if (boss) {
        boss->update(deltaTime, window, player.getPosition(), map);
    }

    for (auto& npc : npcs) {
        npc->update(deltaTime, window, player.getPosition(), map);
        npc->checkCollisionWithPlayer(player);
    }

    // Vérifie la collecte des potions
    for (auto it = potions.begin(); it != potions.end();) {
        if (player.getShape().getGlobalBounds().intersects(it->getGlobalBounds())) {
            player.collectHealthPotion();
            it = potions.erase(it);
        }
        else {
            ++it;
        }
    }
    for (auto it = chest_closed.begin(); it != chest_closed.end();) {
        if (player.getShape().getGlobalBounds().intersects(it->getGlobalBounds()) && player.playerAttacking()) {
                player.collectHealthPotion();
                player.collectHealthPotion();
                Vector2f chestPosition = it->getPosition();
                it = chest_closed.erase(it);
                Sprite chestSprite;
                chestSprite.setTexture(chestOpenTexture);
                chestSprite.setPosition(chestPosition);
                chestSprite.setScale(1.f, 1.f);
                chest_open.push_back(chestSprite);
        }
        else {
            ++it;
        }
    }

    merchant.update(deltaTime, window, player.getPosition(), map);
    merchant.checkCollisionWithPlayer(player);
}

// --- Change de carte ---
void GameState::changeMap(const string& newMapPath, bool useAlternativeSpawn) {
    cout << "Changement de carte vers : " << newMapPath << endl;

    isLoading = true;
    isFading = true;
    fadeIn = true;
    fadeAlpha = 0.0f;
    chaserEnemies.clear();
    patternEnemies.clear();


    while (fadeAlpha < 255.0f) {
        updateFade(0.016f);
        draw();
        window.display();
    }

    if (!lobby) {
        map = Map(newMapPath, "assets/tilesets/Tileset_Grass.png", "assets/tilesets/items.png", 32, { 77, 99, 78 }, { 62, 52, 27, 53, 69, 70, 67, 65, 66, 68, 64 });
    }
    else {
        map = Map("assets/maps/lobby.txt", "assets/tilesets/Tileset_Grass.png", "assets/tilesets/items.png", 32, {65}, { 72, 73, 80, 81, 88, 89 });
    }
    spawnEnemies();
    map.generateTiles();
    map.generateItems();

    //Choisir le bon spawn
    Vector2f newStartPosition = useAlternativeSpawn ? map.getSpawnPoint2() : map.getSpawnPoint1();

    int maxX = map.getWidth() * 32;
    int maxY = map.getHeight() * 32;

    player.setPosition(newStartPosition);


 
    npcs.clear();
    spawnNPCs();

    potions.clear();
    for (const auto& pos : map.getPotionPositions()) {
        Sprite potionSprite;
        potionSprite.setTexture(potionTexture);
        potionSprite.setPosition(pos);
        potionSprite.setScale(1.f, 1.f);
        potions.push_back(potionSprite);
    }

    chest_closed.clear();
    chest_open.clear();
    for (const auto& pos : map.getChestPositions()) {
        Sprite ChestSprite;
        ChestSprite.setTexture(chestClosedTexture);
        ChestSprite.setPosition(pos);
        ChestSprite.setScale(1.f, 1.f);
        chest_closed.push_back(ChestSprite);
    }

    cout << "Changement de carte termine !" << endl;

    player.update(0.0f, window, player.getPosition(), map);

    isFading = true;
    fadeIn = false;
    fadeAlpha = 255.0f;

    isLoading = false;
}

bool GameState::getIsLoading() {
    return isLoading;
}

// --- Transition fondu ---
void GameState::updateFade(float deltaTime) {
    if (!isFading) return;

    if (fadeIn) {
        fadeAlpha += 200.0f * deltaTime;
        if (fadeAlpha >= 255.0f) {
            fadeAlpha = 255.0f;
            isFading = false;
        }
    }
    else {
        fadeAlpha -= 200.0f * deltaTime;
        if (fadeAlpha <= 0.0f) {
            fadeAlpha = 0.0f;
            isFading = false;
        }
    }

    fadeOverlay.setFillColor(Color(0, 0, 0, static_cast<Uint8>(fadeAlpha)));
}


// --- Dessine l'état du jeu ---
void GameState::draw() {

    window.setView(player.getCameraView());

    map.draw(window);


    player.draw(window);

    for (auto& enemy : chaserEnemies) {
        enemy->draw(window);
    }

    for (auto& enemy : patternEnemies) {
        enemy->draw(window);
    }

    for (auto& npc : npcs) {
        npc->draw(window);
    }

    for (auto& potion : potions) {
        window.draw(potion);
    }

    for (auto& chest : chest_closed) {
        window.draw(chest);
    }

    for (auto& chest : chest_open) {
        window.draw(chest);
    }

    boss->draw(window);

    View hudView(FloatRect(0, 0, window.getSize().x, window.getSize().y));
    window.setView(hudView);
    hud.draw(window);
    window.draw(fadeOverlay);
}