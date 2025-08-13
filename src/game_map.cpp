#include "game_map.h"

GameMap::GameMap(const char *name) {

	mapFileName = name;
}

bool GameMap::readMapFile() {

	std::fstream file(mapFileName, std::ios::in);
	if (!file.good()) {
		std::cout << "couldnt open file " << mapFileName;
		return 0;
	}

	mapData.push_back(std::vector<int>());
	char code;
	while (file >> std::noskipws >> code) {
		if (code == '\n') {
			
			mapData.push_back(std::vector<int>());
		}
		else if (code == 'p') {
			playerStartPosition.y = (mapData.size() - 1) + .5;
			playerStartPosition.x = mapData[mapData.size() - 1].size() + .5;
			mapData[mapData.size() - 1].push_back(0);
		}
		else if (code == 'e') {
			enemyStartPosition.y = (mapData.size() - 1) + .5;
			enemyStartPosition.x = mapData[mapData.size() - 1].size() + .5;
			mapData[mapData.size() - 1].push_back(0);
			
		}
		else {
			mapData[mapData.size() - 1].push_back(code - '0');
		}
	}
	mapHeight = mapData.size();
	mapWidth = mapData[0].size();

	/*for (int i = 0; i < mapData.size(); i++) {
		for (int j = 0; j < mapData[i].size(); j++) {
			std::cout << mapData[i][j] << " ";
		}
		std::cout << std::endl;
	}*/
	return 1;
}