﻿#include "../include/Map.h"
#include <fstream>
#include <iostream>

Map::Map(const string& filename, const string& tilesetPath, const string& itemsetPath, int tileSize, vector<int> blockedTileValues, vector<int> blockedItemValues)
    : tileSize(tileSize), blockedTileValues(blockedTileValues), blockedItemValues(blockedItemValues) {

    // Chargement des textures
    if (!tileSet.loadFromFile(tilesetPath)) {
        cerr << "Erreur de chargement du tileset principal" << endl;
    }
    if (!itemSet.loadFromFile(itemsetPath)) {
        cerr << "Erreur de chargement du tileset des items" << endl;
    }

    // Chargement de la carte
    loadFromFile(filename);

    // Génération des graphismes
    generateTiles();
    generateItems();
}


// CHARGEMENT DU FICHIER DE LA CARTE

void Map::loadFromFile(const string& filename) {
    ifstream file(filename);
    if (!file) {
        cerr << "Erreur d'ouverture du fichier de la carte" << endl;
        return;
    }

    string line;
    int layer = 0;  // 0 = Ground Layer, 1 = Items Layer, 2 = Entity Layer
    int currentY = 0;

    // Stockage temporaire des calques
    vector<vector<int>> tempTileMap;
    vector<vector<int>> tempItemMap;
    vector<vector<int>> tempEntityMap;

    vector<Vector2i> tempBlockedTiles;
    vector<Vector2i> tempBlockedItemTiles;

    vector<Vector2f> tempEnemyPositions;
    vector<Vector2f> tempNpcPositions;
    Vector2f tempPlayerStartPosition;

    while (getline(file, line)) {
        // Détection des sections du fichier
        if (line.find("# Items Layer") != string::npos) {
            layer = 1; currentY = 0;
            continue;
        }
        else if (line.find("# Entity Layer") != string::npos) {
            layer = 2; currentY = 0;
            continue;
        }

        vector<int> row;
        stringstream ss(line);
        int tile;
        int x = 0;

        while (ss >> tile) {
            row.push_back(tile);

            if (layer == 0) {  // Ground Layer
                if (find(blockedTileValues.begin(), blockedTileValues.end(), tile) != blockedTileValues.end()) {
                    tempBlockedTiles.push_back(Vector2i(x, currentY));
                }
            }
            else if (layer == 1) {  // Items Layer
                if (find(blockedItemValues.begin(), blockedItemValues.end(), tile) != blockedItemValues.end()) {
                    tempBlockedItemTiles.push_back(Vector2i(x, currentY));
                }
                //Détection des potions (ID 22)
                if (tile == 22) {
                    potionPositions.push_back(Vector2f(x * tileSize, currentY * tileSize));
                    cout << "Potion ajoutee à la position (" << x << ", " << currentY << ")" << endl;  // Debug
                }
            }
            else if (layer == 2) {  // Entity Layer
                if (tile == 1) {  // Joueur
                    tempPlayerStartPosition = Vector2f(x * tileSize, currentY * tileSize);
                }
                else if (tile == 2) {  // Ennemi
                    tempEnemyPositions.push_back(Vector2f(x * tileSize, currentY * tileSize));
                }
                else if (tile == 3) {  // NPC
                    tempNpcPositions.push_back(Vector2f(x * tileSize, currentY * tileSize));
                }
            }

            x++;
        }

        //Ajout des lignes aux calques correspondants
        if (layer == 0) tempTileMap.push_back(row);
        else if (layer == 1) tempItemMap.push_back(row);
        else if (layer == 2) tempEntityMap.push_back(row);

        currentY++;
    }

    // Assignation finale
    tileMap = tempTileMap;
    itemMap = tempItemMap;
    entityMap = tempEntityMap;
    blockedTiles = tempBlockedTiles;
    blockedItemTiles = tempBlockedItemTiles;
    enemyPositions = tempEnemyPositions;
    npcPositions = tempNpcPositions;
    playerStartPosition = tempPlayerStartPosition;
}


// GÉNÉRATION DES OBJETS

void Map::generateItems() {
    int itemsetWidth = itemSet.getSize().x / tileSize;

    for (size_t y = 0; y < itemMap.size(); ++y) {
        for (size_t x = 0; x < itemMap[y].size(); ++x) {
            int itemIndex = itemMap[y][x];

            if (itemIndex > 0) {
                int tileX = (itemIndex % itemsetWidth) * tileSize;
                int tileY = (itemIndex / itemsetWidth) * tileSize;

                Sprite sprite;
                sprite.setTexture(itemSet);
                sprite.setTextureRect(IntRect(tileX, tileY, tileSize, tileSize));
                sprite.setPosition(x * tileSize, y * tileSize);
                items.push_back(sprite);
            }
        }
    }
}


// GÉNÉRATION DES TUILES

void Map::generateTiles() {
    int tilesetWidth = tileSet.getSize().x / tileSize;

    for (size_t y = 0; y < tileMap.size(); ++y) {
        for (size_t x = 0; x < tileMap[y].size(); ++x) {
            int tileIndex = tileMap[y][x];

            int tileX = (tileIndex % tilesetWidth) * tileSize;
            int tileY = (tileIndex / tilesetWidth) * tileSize;

            Sprite sprite;
            sprite.setTexture(tileSet);
            sprite.setTextureRect(IntRect(tileX, tileY, tileSize, tileSize));
            sprite.setPosition(x * tileSize, y * tileSize);
            tiles.push_back(sprite);
        }
    }
}


// COLLISIONS

bool Map::isWalkable(Vector2f position, Vector2f playerSize, FloatRect hitboxBounds) {
    // Vérifier la collision avec les tiles
    for (const auto& tile : blockedTiles) {
        FloatRect tileBounds(tile.x * tileSize, tile.y * tileSize, tileSize, tileSize);
        if (hitboxBounds.intersects(tileBounds)) {
            return false;
        }
    }

    // Vérifie la collision avec les items
    for (const auto& item : blockedItemTiles) {
        FloatRect itemBounds(item.x * tileSize, item.y * tileSize, tileSize, tileSize);
        if (hitboxBounds.intersects(itemBounds)) {
            return false;
        }
    }

    return true;
}

// AFFICHAGE DE LA CARTE

void Map::draw(RenderWindow& window) {
    for (const auto& tile : tiles) window.draw(tile);
    for (const auto& item : items) window.draw(item);
}
