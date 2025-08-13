#include "enemy.h"

Enemy::Enemy() {

	seekPath = 1;
	mMoveSpeed = 4;
	soundWarning = 0;
	encounterWarningClock.restart();
}

void Enemy::setSounds(sf::SoundBuffer(&sounds)[5]) {
	behindWarning.setBuffer(sounds[3]);
	behindWarning.setVolume(50);

	frontWarning.setBuffer(sounds[4]);
	frontWarning.setVolume(50);
}

bool Enemy::checkCollision(Player player) {

	sf::FloatRect enemyRect(position.x - 0.4, position.y - 0.4, 0.8, 0.8);
	if (enemyRect.contains(player.position)) {
		return 1;
	}
	return 0;
}

void Enemy::update(sf::Time dt) {

	if (mPath.empty()) {
		seekPath = 1;
		return;
	}

	sf::Vector2f direction(mPath[0].x - position.x, mPath[0].y - position.y);

	//normalize vector to get direction vector
	direction = [direction]() {
		float length = sqrt(direction.x * direction.x + direction.y * direction.y);
		if (length == 0) {
			return sf::Vector2f(0, 0);
		}
		return sf::Vector2f(direction.x / length, direction.y / length);
	}();

	sf::Vector2f velocity(mMoveSpeed * direction.x * dt.asSeconds(), mMoveSpeed * direction.y * dt.asSeconds());
	position += velocity;

	//if enemy walked through that point then move it to it
	if (abs(position.x - mPath[0].x) < velocity.x) {
		position.x = mPath[0].x;
		if (mPath.size() > 2) {
			if (mPath[0].x != mPath[1].x) {
				seekPath = 1;
			}
		}
	}
	if (abs(position.y - mPath[0].y) < velocity.y) {
		position.y = mPath[0].y;
		if (mPath.size() > 2) {
			if (mPath[0].y != mPath[1].y) {
				seekPath = 1;
			}
		}
	}
	
	//delete path node if enemy walked through it 
	if (position == mPath[0]) {
		mPath.erase(mPath.begin());
	}
}

void prepareValues(std::vector<std::vector<fieldValues>> &mapFieldValues, GameMap gameMap) {

	for (int x = 0; x < gameMap.mapWidth; x++) {
		mapFieldValues.push_back(std::vector<fieldValues>());
		for (int y = 0; y < gameMap.mapWidth; y++) {
			mapFieldValues[x].push_back(fieldValues());
		}
	}
}

void Enemy::findPathToPlayer(sf::Vector2f playerPos, GameMap gameMap) {

	std::priority_queue<fieldValues*, std::vector<fieldValues*>, fieldValues> openset;
	std::vector<std::vector<fieldValues>> mapFieldsValues;
	prepareValues(mapFieldsValues, gameMap);

	sf::Vector2i playerPosField(playerPos.x, playerPos.y);
	sf::Vector2i enemyPosField(position.x, position.y);

	mapFieldsValues[enemyPosField.x][enemyPosField.y].gScore = 0;
	mapFieldsValues[enemyPosField.x][enemyPosField.y].fScore = countDistance(enemyPosField, playerPosField);
	mapFieldsValues[enemyPosField.x][enemyPosField.y].fieldNumber = enemyPosField;
	mapFieldsValues[enemyPosField.x][enemyPosField.y].wasVisited = 1;

	openset.push(&mapFieldsValues[enemyPosField.x][enemyPosField.y]);

	//A* algorithm 
	while (openset.size() != 0) {
		fieldValues* current = openset.top();
		if (current->fieldNumber.x == playerPosField.x && current->fieldNumber.y == playerPosField.y) {
			reconstructPath(enemyPosField, playerPos, mapFieldsValues);
			return;
		}

		//prepare neighbours coords
		sf::Vector2i neighbours[4] = {
			sf::Vector2i(0, -1),
			sf::Vector2i(-1, 0),
			sf::Vector2i(1, 0),
			sf::Vector2i(0, 1)
		};

		for (int i = 0; i < 4; i++) {
			sf::Vector2i neighbourCoords = current->fieldNumber + neighbours[i];
			mapFieldsValues[neighbourCoords.x][neighbourCoords.y].fieldNumber = neighbourCoords;

			//check if neighbour coords are not outside the map
			if (neighbourCoords.x > gameMap.mapWidth ||
				neighbourCoords.x < 0 ||
				neighbourCoords.y > gameMap.mapHeight ||
				neighbourCoords.y < 0) {
				continue;
			}
			//check if neighbour is not a wall
			if (gameMap.mapData[neighbourCoords.y][neighbourCoords.x] != 0) {
				
				continue;
			}

			int tentativeGScore = current->gScore + 1;
			if (tentativeGScore < mapFieldsValues[neighbourCoords.x][neighbourCoords.y].gScore) {
				mapFieldsValues[neighbourCoords.x][neighbourCoords.y].cameFrom = current->fieldNumber;

				mapFieldsValues[neighbourCoords.x][neighbourCoords.y].gScore = tentativeGScore;
				mapFieldsValues[neighbourCoords.x][neighbourCoords.y].fScore = tentativeGScore + countDistance(neighbourCoords, playerPosField);

				if (!mapFieldsValues[neighbourCoords.x][neighbourCoords.y].wasVisited) {
					openset.push(&mapFieldsValues[neighbourCoords.x][neighbourCoords.y]);
					mapFieldsValues[neighbourCoords.x][neighbourCoords.y].wasVisited = 1;
				}
			}
		}
		openset.pop();
	}
}

int Enemy::countDistance(sf::Vector2i start, sf::Vector2i finish) {

	return abs(start.x - finish.x) + abs(start.y - finish.y);
}

void Enemy::reconstructPath(sf::Vector2i enemyPosField, sf::Vector2f playerPos, std::vector<std::vector<fieldValues>> mapFieldsValues) {

	//path contains points on the map which enemy has to visit
	mPath.clear();
	mPath.push_back(playerPos);

	int x = playerPos.x;
	int y = playerPos.y;
	while (x != enemyPosField.x || y != enemyPosField.y) {

		int previousFieldX = mapFieldsValues[x][y].cameFrom.x;
		int preciousFieldY = mapFieldsValues[x][y].cameFrom.y;
		mPath.insert(mPath.begin(), sf::Vector2f(previousFieldX + .5, preciousFieldY + .5));

		x = previousFieldX;
		y = preciousFieldY;
	}
	seekPath = 0;
}