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
    //Calques de la carte
    vector<vector<int>> tileMap;      // Calque des tuiles
    vector<vector<int>> itemMap;      // Calque des objets
    vector<vector<int>> entityMap;    // Calque des entités

    //Blocages (collision)
    vector<int> blockedTileValues;
    vector<int> blockedItemValues;
    vector<Vector2i> blockedTiles;
    vector<Vector2i> blockedItemTiles;

    //item speciaux
    vector<Vector2f> potionPositions;

    //Entités du jeu
    vector<Vector2f> enemyPositions;
    vector<Vector2f> npcPositions;
    Vector2f playerStartPosition;

    //Graphiques
    Texture tileSet;
    Texture itemSet;
    vector<Sprite> tiles;
    vector<Sprite> items;

    int tileSize;

    //Fonctions de chargement
    void loadFromFile(const string& filename);

public:
    //Constructeur
    Map(const string& filename, const string& tilesetPath, const string& itemsetPath, int tileSize, vector<int> blockedTileValues, vector<int> blockedItemValues);
    void generateTiles();
    void generateItems();

    //Guetters
    vector<Vector2f> getEnemyPositions() const { return enemyPositions; }
    Vector2f getPlayerStartPosition() const { return playerStartPosition; }
    vector<Vector2f> getNpcPositions() const { return npcPositions; }
    vector<Vector2f> getPotionPositions() const { return potionPositions; }
    int getTileAt(const Vector2f& position);
    int getItemAt(const Vector2f& position);
    int getWidth() const { return tileMap.size(); }
    int getHeight() const { return (tileMap.empty() ? 0 : tileMap[0].size()); }
    int getTileCount() const { return tiles.size(); }
    int getItemCount() const { return items.size(); }



    //Fonctions principales
    void draw(RenderWindow& window);
    bool isWalkable(Vector2f position, Vector2f playerSize, FloatRect hitboxBounds);
};

#endif
