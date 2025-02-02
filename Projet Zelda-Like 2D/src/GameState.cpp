#include "../include/GameState.h"
#include "../include/Merchant.h"

using namespace sf;
using namespace std;

// --- Constructeur ---
GameState::GameState(RenderWindow& window, Player& player, int gameState)
    : State(window),
    player(player),
    merchant(450, 190),
    map("assets/maps/lobby.txt", "assets/tilesets/Tileset_Grass.png", "assets/tilesets/items.png", 32, {}, { 72, 73, 80, 81, 88, 89 }),
    gameState(gameState),
    lobby(true),
    hud(player) {

    Merchant::loadMerchantTexture("assets/NPC/merchant.png");

    if (!ambientSound.openFromFile("assets/music/dungeon_ambient_1.ogg")) {
        std::cerr << "Erreur de chargement du son d'ambiance !\n";
    }
    else {
        ambientSound.setLoop(true);
        ambientSound.setVolume(5);
        ambientSound.play();
    }

    if (!potionTexture.loadFromFile("assets/HUD/health_potion.png")) {
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

// --- Génère les ennemis ---
void GameState::spawnEnemies() {
    for (const auto& pos : map.getEnemyPositions()) {
        enemies.emplace_back(pos.x, pos.y, 50.0f);
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

    //Vérifie si le joueur marche sur la tile 63
    int itemID = map.getItemAt(player.getPosition());

    if (itemID == -1) {
        cerr << "Erreur : Item inexistant à cette position !" << endl;
        return;
    }

    if (itemID == 63) { //TP vers le donjon
        changeMap("assets/maps/dungeon.txt", false);
    }

    if (itemID == 60) { //Retour au lobby
        lobby = true;
        changeMap("assets/maps/lobby.txt", true);
    }

    if (itemID == 61) { //TP vers la map principale
        lobby = false;
        changeMap("assets/maps/map.txt", false);
    }

    if (itemID == 51) { //TP vers la map principale (alternatif)
        changeMap("assets/maps/map.txt", true);
    }

    for (auto& enemy : enemies) {
        enemy.update(deltaTime, window, player.getPosition(), map);
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

    while (fadeAlpha < 255.0f) {
        updateFade(0.016f);
        draw();
        window.display();
    }

    if (!lobby) {
        map = Map(newMapPath, "assets/tilesets/tiles.png", "assets/tilesets/items.png", 32, { 6, 99, 5 }, { 62, 64 });
    }
    else {
        map = Map("assets/maps/lobby.txt", "assets/tilesets/Tileset_Grass.png", "assets/tilesets/items.png", 32, {}, { 72, 73, 80, 81, 88, 89 });
    }

    map.generateTiles();
    map.generateItems();

    //Choisir le bon spawn
    Vector2f newStartPosition = useAlternativeSpawn ? map.getSpawnPoint2() : map.getSpawnPoint1();

    int maxX = map.getWidth() * 32;
    int maxY = map.getHeight() * 32;

    player.setPosition(newStartPosition);

    enemies.clear();
    spawnEnemies();
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

    cout << "Changement de carte termine !" << endl;

    player.update(0.0f, window, player.getPosition(), map);

    isFading = true;
    fadeIn = false;
    fadeAlpha = 255.0f;

    isLoading = false;
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

    for (auto& enemy : enemies) {
        enemy.draw(window);
    }

    for (auto& npc : npcs) {
        npc->draw(window);
    }

    for (auto& potion : potions) {
        window.draw(potion);
    }

    // Définit la vue pour l'interface HUD
    View hudView(FloatRect(0, 0, window.getSize().x, window.getSize().y));
    window.setView(hudView);
    hud.draw(window);
    window.draw(fadeOverlay);
}
