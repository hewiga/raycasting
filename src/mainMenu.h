#pragma once
#include "screen.h"
#include "game.h"
#include "textButton.h"

class MainMenu : public Screen {
	sf::RenderWindow *m_window;
	sf::RenderTexture *m_canvas;

	sf::Font* m_font;
	TextButton m_infoButton;
	TextButton m_playButton;
	TextButton m_exitButton;

	int m_windowsWidth;

public:
	MainMenu(sf::RenderWindow &window, sf::RenderTexture &canvas, sf::Font& textFont);
	~MainMenu();
	void update(std::vector<std::unique_ptr<Screen>> &screens, sf::Time dt) override;
	void handleKeyboard(sf::Time dt) override;
	void handleEvents(std::vector<std::unique_ptr<Screen>> &screens, sf::Time dt) override;
};