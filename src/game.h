#pragma once
#include <SFML/Graphics.hpp>
#include "screen.h"
#include "player.h"
#include "enemy.h"
#include "game_map.h"
#include "mainMenu.h"

class Game : public Screen {

	void raycasting();
	void drawSprites();
	int loadFiles();

	sf::RenderWindow* mWindow;
	sf::RenderTexture* mCanvas;
	sf::Font* mFont;

	int mWindowsWidth;
	int mWindowsHeight;
	const int mTEXTURE_SIZE = 64;
	std::unique_ptr<double[]> mZBuffer;
	sf::Texture mTextures[6];
	sf::SoundBuffer mSounds[5];
	GameMap mGameMap;
	Player mPlayer;
	Enemy mEnemy;

public:
	Game(sf::RenderWindow& window, sf::RenderTexture& canvas, sf::Font& textFont);
	~Game();
	void update(std::vector<std::unique_ptr<Screen>> &screens, sf::Time dt) override;
	void handleKeyboard(sf::Time dt) override;
	void handleEvents(std::vector<std::unique_ptr<Screen>>& screens, sf::Time dt) override;
};