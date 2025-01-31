#include "../include/Map.h"
#include <fstream>
#include <iostream>

Map::Map(const string& filename, const string& tilesetPath, const string& itemsetPath, int tileSize, vector<int> blockedTileValues, vector<int> blockedItemValues)
    : tileSize(tileSize), blockedTileValues(blockedTileValues), blockedItemValues(blockedItemValues) {
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
    int y = 0;
    bool readingItems = false;

    while (getline(file, line)) {
        if (line.find("# Items Layer") != string::npos) {
            readingItems = true;
            continue;
        }

        vector<int> row;
        stringstream ss(line);
        int tile;
        int x = 0;

        while (ss >> tile) {
            row.push_back(tile);

            if (!readingItems && find(blockedTileValues.begin(), blockedTileValues.end(), tile) != blockedTileValues.end()) {
                blockedTiles.push_back(Vector2i(x, y));
            }
            if (readingItems && find(blockedItemValues.begin(), blockedItemValues.end(), tile) != blockedItemValues.end()) {
                blockedItemTiles.push_back(Vector2i(x, y));
            }

            x++;
        }

        if (readingItems) {
            itemMap.push_back(row);
        }
        else {
            tileMap.push_back(row);
        }
        y++;
    }
}


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

bool Map::isWalkable(Vector2f position, Vector2f playerSize, FloatRect hitboxBounds) {
    for (auto& tile : blockedTiles) {
        FloatRect tileBounds(tile.x * tileSize, tile.y * tileSize, tileSize, tileSize);
        if (hitboxBounds.intersects(tileBounds)) {
            return false;
        }
    }

    for (auto& item : blockedItemTiles) {
        FloatRect itemBounds(item.x * tileSize, item.y * tileSize, tileSize, tileSize);

        if (hitboxBounds.intersects(itemBounds)) {
            cout << "Collision detected!" << endl;
            return false;
        }
    }

    return true;
}


void Map::draw(RenderWindow& window) {
    for (const auto& tile : tiles) {
        window.draw(tile);
    }
    for (const auto& item : items) {
        window.draw(item);
    }
}