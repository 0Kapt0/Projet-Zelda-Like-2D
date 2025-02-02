#include "../include/GameState.h"
#include "../include/Merchant.h"

using namespace sf;
using namespace std;

// --- Constructeur ---
GameState::GameState(RenderWindow& window, Player& player, int gameState)
    : State(window),
    player(player),
    merchant(450, 190),
    map("assets/maps/map.txt", "assets/tilesets/tiles.png", "assets/tilesets/items.png", 32, { 6, 99, 5 }, { 62, 64 }),
    fence("assets/maps/fence.txt", "assets/tilesets/fence.png", "assets/tilesets/items.png", 32, {}, {}),
    gameState(gameState),
    hud(player) {

    Merchant::loadMerchantTexture("assets/NPC/merchant.png");

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

    //Initialise le joueur et les entités
    player.setPosition(map.getPlayerStartPosition());
    spawnEnemies();
    spawnNPCs();
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

    //Vérifie si le joueur marche sur la tile 63
    int itemID = map.getItemAt(player.getPosition());

    if (itemID == 63) {  // Vérifie si l'item est bien la tuile de téléportation
        cout << "Téléportation détectée !\n";
        changeMap("assets/maps/dungeon.txt");
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
void GameState::changeMap(const string& newMapPath) {
    cout << "Changement de carte vers : " << newMapPath << endl;

    //Charge la nouvelle carte
    map = Map(newMapPath, "assets/tilesets/tiles.png", "assets/tilesets/items.png", 32, { 6, 99, 5 }, { 62, 64 });

    if (map.getWidth() == 0 || map.getHeight() == 0 || map.getTileCount() == 0) {
        cerr << "Erreur : Impossible de charger la nouvelle carte !" << endl;
        return;
    }

    //Régénére les tiles et items après changement de carte
    map.generateTiles();
    map.generateItems();

    //Vérifie si les tiles sont bien générées
    if (map.getTileCount() == 0) {
        cerr << "Erreur : Aucune tuile générée après le changement de carte !" << endl;
    }

    //Mettre le joueur à une position valide
    Vector2f newStartPosition = map.getPlayerStartPosition();
    int maxX = map.getWidth() * 32;
    int maxY = map.getHeight() * 32;

    if (newStartPosition.x < 0 || newStartPosition.x >= maxX ||
        newStartPosition.y < 0 || newStartPosition.y >= maxY) {
        cerr << "Erreur : Position de spawn invalide !\n";
        newStartPosition = { 32, 32 };
    }

    player.setPosition(newStartPosition);

    //Nettoyer et recharger les entités
    enemies.clear();
    spawnEnemies();

    npcs.clear();
    spawnNPCs();

    //Recharger les potions
    potions.clear();
    for (const auto& pos : map.getPotionPositions()) {
        Sprite potionSprite;
        potionSprite.setTexture(potionTexture);
        potionSprite.setPosition(pos);
        potionSprite.setScale(1.f, 1.f);
        potions.push_back(potionSprite);
    }

    cout << "Changement de carte terminé avec succès !" << endl;
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
}
