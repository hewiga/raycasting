#pragma once
#include "screen.h"
#include "textButton.h"
#include "game.h"

class GameOver : public Screen {

	sf::RenderWindow* m_window;
	sf::RenderTexture* m_canvas;
	
	sf::Font* m_font;
	TextButton m_infoText;
	TextButton m_restartButton;
	TextButton m_mainMenuButton;
	TextButton m_exitButton;

	int m_windowsWidth;

public:
	GameOver(sf::RenderWindow& window, sf::RenderTexture& canvas, sf::Font& textFont, const char* title);
	virtual void update(std::vector<std::unique_ptr<Screen>> &screens, sf::Time dt) override;
	virtual void handleKeyboard(sf::Time dt) override;
	virtual void handleEvents(std::vector<std::unique_ptr<Screen>>& screens, sf::Time dt) override;
};