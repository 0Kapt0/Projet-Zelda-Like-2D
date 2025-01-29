#include "../include/Map.h"
#include <fstream>
#include <iostream>

Map::Map(const string& filename, const string& tilesetPath, int tileSize)
    : tileSize(tileSize) {
    if (!tileSet.loadFromFile(tilesetPath)) {
        cerr << "Erreur de chargement du tileset" << endl;
    }
    loadFromFile(filename);
    generateTiles();
}

void Map::loadFromFile(const string& filename) {
    ifstream file(filename);
    if (!file) {
        cerr << "Erreur d'ouverture du fichier de la carte" << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        vector<int> row;
        stringstream ss(line);
        int tile;

        while (ss >> tile) {
            row.push_back(tile);
        }
        tileMap.push_back(row);
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
}