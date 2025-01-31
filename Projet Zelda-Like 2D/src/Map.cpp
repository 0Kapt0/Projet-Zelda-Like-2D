#include "../include/Map.h"
#include <fstream>
#include <iostream>

Map::Map(const string& filename, const string& tilesetPath, const string& itemsetPath, int tileSize)
    : tileSize(tileSize) {
    if (!tileSet.loadFromFile(tilesetPath)) {
        cerr << "Erreur de chargement du tileset principal" << endl;
    }
    if (!itemSet.loadFromFile(itemsetPath)) {
        cerr << "Erreur de chargement du tileset des items" << endl;
    }
    loadFromFile(filename);
    generateTiles();
    generateItems();
}

void Map::loadFromFile(const string& filename) {
    ifstream file(filename);
    if (!file) {
        cerr << "Erreur d'ouverture du fichier de la carte" << endl;
        return;
    }

    string line;
    bool readingItems = false;

    while (getline(file, line)) {
        if (line.find("# Items Layer") != string::npos) {
            readingItems = true;
            continue;
        }

        vector<int> row;
        stringstream ss(line);
        int tile;

        while (ss >> tile) {
            row.push_back(tile);
        }

        if (readingItems) {
            itemMap.push_back(row);
        }
        else {
            tileMap.push_back(row);
        }
    }
}

void Map::generateItems() {
    int itemsetWidth = itemSet.getSize().x / tileSize;

    for (size_t y = 0; y < itemMap.size(); ++y) {
        for (size_t x = 0; x < itemMap[y].size(); ++x) {
            int itemIndex = itemMap[y][x];

            if (itemIndex > 0) { // 🎯 Ignore les cases vides
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


void Map::draw(RenderWindow& window) {
    for (const auto& tile : tiles) {
        window.draw(tile);
    }
    for (const auto& item : items) {
        window.draw(item);
    }
}