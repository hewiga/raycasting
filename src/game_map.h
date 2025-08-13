#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <fstream>
#include <vector>
#include <iostream>

class GameMap {
public:

	const char *mapFileName;
	std::vector <std::vector<int>> mapData;
	sf::Vector2f playerStartPosition;
	sf::Vector2f enemyStartPosition;
	int mapWidth;
	int mapHeight;

	GameMap(const char *name);
	bool readMapFile();
};