#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include "game_map.h"


class Player {
public:
	sf::Vector2f position;
	sf::Vector2f direction;
	sf::Vector2f plane;
	sf::Sound walkSound;
	sf::Sound gameSound;
	int moveSpeed;
	int rotateSpeed;

	Player();
	void setSounds(sf::SoundBuffer(&sounds)[5]);
	void move(sf::Time dt, int button, GameMap gameMap);
	void rotate(sf::Time dt, int button);
	void rotate(sf::Time dt, int mouseX, int WINDOWS_WIDTH);
	bool interact(GameMap gameMap);
};