#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <queue>
#include "game_map.h"
#include "player.h"

struct fieldValues {
	int gScore = INT_MAX - 1; //distance between enemys position and current field
	int fScore = INT_MAX;	  //distance between players position and current field + fScore
	bool wasVisited = 0;
	sf::Vector2i cameFrom;
	sf::Vector2i fieldNumber;

	bool operator()(fieldValues* f1, fieldValues* f2) {
		return f1->fScore > f2->fScore;
	}
};

class Enemy {

	std::vector<sf::Vector2f> mPath;
	int mMoveSpeed;

	int countDistance(sf::Vector2i start, sf::Vector2i finish);
	void reconstructPath(sf::Vector2i enemyPosField, sf::Vector2f playerPos, std::vector<std::vector<fieldValues>> mapFieldsValues);

public:
	sf::Vector2f position;
	sf::Sound behindWarning;
	sf::Sound frontWarning;

	sf::Clock encounterWarningClock;

	bool seekPath;
	bool soundWarning;

	Enemy();
	void setSounds(sf::SoundBuffer(&sounds)[5]);
	void update(sf::Time dt);
	bool checkCollision(Player player);
	void findPathToPlayer(sf::Vector2f playerPos, GameMap gameMap);
	
};