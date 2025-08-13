#include "player.h"

Player::Player() {

	direction.x = 0;
	direction.y = -1;
	
	plane.x = 0.66;
	plane.y = 0;

	moveSpeed = 3;
	rotateSpeed = 1;	
}

void Player::setSounds(sf::SoundBuffer (&sounds)[5]) {
	walkSound.setBuffer(sounds[0]);

	gameSound.setBuffer(sounds[2]);
	gameSound.play();
	gameSound.setLoop(true);
}

void Player::move(sf::Time dt, int button, GameMap gameMap) {

	//check X axis collision
	float new_position_x = position.x + moveSpeed * button * direction.x * dt.asSeconds();
	if (new_position_x < position.x) {
		//if player is moving left
		if ((gameMap.mapData[int((position.y + .1))][int((new_position_x - .1))] != 0) ||
			(gameMap.mapData[int((position.y - .1))][int((new_position_x - .1))] != 0)) {
			new_position_x = (floor((new_position_x - .1)) + 1) + .11;
		}
	}
	else if (new_position_x > position.x){
		//if player is moving right
		if ((gameMap.mapData[int((position.y + .1))][int((new_position_x + .1))] != 0)||
			(gameMap.mapData[int((position.y - .1))][int((new_position_x + .1))] != 0)) {
			new_position_x = int((new_position_x + .1)) - .11;
		}
	}

	//check Y axis collision
	float new_position_y = position.y + moveSpeed * button * direction.y * dt.asSeconds();
	if (new_position_y < position.y) {
		//if player is moving up
		if ((gameMap.mapData[int((new_position_y - .1))][int((position.x - .1))] != 0) ||
			(gameMap.mapData[int((new_position_y - .1))][int((position.x + .1))] != 0)) {
			new_position_y = (floor((new_position_y - .1)) + 1) + .11;
		}
	}
	else if (new_position_y > position.y){
		//if player is moving down
		if ((gameMap.mapData[int((new_position_y + .1))][int((position.x - .1))] != 0) ||
			(gameMap.mapData[int((new_position_y + .1))][int((position.x + .1))] != 0)) {
			new_position_y = int((new_position_y + .1)) - .11;
		}
	}
	position.y = new_position_y;
	position.x = new_position_x;
}

void Player::rotate(sf::Time dt, int mouseX, int WINDOWS_WIDTH) {

	int rotationSize = (mouseX - (WINDOWS_WIDTH / 2)) / 5;
	double oldDirX = direction.x;
	direction.x = direction.x * cos(rotationSize * dt.asSeconds()) - direction.y * sin(rotationSize * dt.asSeconds());
	direction.y = oldDirX * sin(rotationSize * dt.asSeconds()) + direction.y * cos(rotationSize * dt.asSeconds());

	double oldPlaneX = plane.x;
	plane.x = plane.x * cos(rotationSize * dt.asSeconds()) - plane.y * sin(rotationSize * dt.asSeconds());
	plane.y = oldPlaneX * sin(rotationSize * dt.asSeconds()) + plane.y * cos(rotationSize * dt.asSeconds());

	sf::Mouse::setPosition(sf::Vector2i(WINDOWS_WIDTH / 2, 560));
}

bool Player::interact(GameMap gameMap) {

	//interaction range is half of field
	sf::Vector2i interactionField(position.x + direction.x * .5, position.y + direction.y * .5);
	
	//if interaction point is inside door field then return true
	if (gameMap.mapData[interactionField.y][interactionField.x] == 5) {
		return 1;
	}
	return 0;
}
