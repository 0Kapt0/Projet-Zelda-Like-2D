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
    Texture tileSet;
    vector<Sprite> tiles;
    int tileSize;

    void loadFromFile(const string& filename);
    void generateTiles();

public:
    Map(const string& filename, const string& tilesetPath, int tileSize);
    void draw(RenderWindow& window);
};

#endif
