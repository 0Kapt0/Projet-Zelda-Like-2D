#ifndef MAP_H
#define MAP_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>

using namespace sf;
using namespace std;

class Map {
private:
    vector<vector<int>> tileMap;
    vector<int> blockedTileValues;
    vector<vector<int>> itemMap;
    Texture tileSet;
    Texture itemSet;
    vector<Sprite> tiles;
    vector<Sprite> items;
    vector<Vector2i> blockedTiles;

    int tileSize;

    void loadFromFile(const string& filename);
    void generateTiles();
    void generateItems();
public:
    Map(const string& filename, const string& tilesetPath, const string& itemsetPath, int tileSize, vector<int> blockedTileValues);
    void draw(RenderWindow& window);
    bool isWalkable(Vector2f position, Vector2f playerSize);
};

#endif
