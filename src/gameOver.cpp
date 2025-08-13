#include <SFML/Graphics.hpp>
#include "gameOver.h"

GameOver::GameOver(sf::RenderWindow& window, sf::RenderTexture& canvas, sf::Font& textFont, const char* title) :
	m_restartButton("Restart", textFont, sf::Color::Red, sf::Color::White),
	m_exitButton("Exit", textFont, sf::Color::Red, sf::Color::White),
	m_infoText(title, textFont, sf::Color::White, sf::Color::White),
	m_mainMenuButton("Go back to menu", textFont, sf::Color::Red, sf::Color::White)
{
	m_font = &textFont;
	m_window = &window;
	m_canvas = &canvas;

	m_windowsWidth = sf::VideoMode::getDesktopMode().width;

	m_infoText.m_textSprite.setCharacterSize(50);
	m_infoText.setPosition(sf::Vector2f(m_windowsWidth / 2, 100));

	m_restartButton.setPosition(sf::Vector2f(m_windowsWidth / 2, 400));
	m_mainMenuButton.setPosition(sf::Vector2f(m_windowsWidth / 2, 500));
	m_exitButton.setPosition(sf::Vector2f(m_windowsWidth / 2, 600));
}

void GameOver::handleEvents(std::vector<std::unique_ptr<Screen>>& screens, sf::Time dt) {

	sf::Event event;
	while (m_window->pollEvent(event)) {
		if (event.type == sf::Event::Closed) {
			m_window->close();
		}
		if (event.type == sf::Event::MouseButtonPressed) {
			if (event.mouseButton.button == sf::Mouse::Left) {
				if (m_exitButton.m_contains) {
					m_window->close();
				}
				else if (m_restartButton.m_contains) {
					//add new game instantion at the beginning
					screens.insert(screens.begin(), std::make_unique<Game>(*m_window, *m_canvas, *m_font));
					//then delete itself (first element in vector is game so "begin() + 1")
					screens.erase(screens.begin() + 1);
					return;
				}
				else if (m_mainMenuButton.m_contains) {
					//delete itself
					screens.erase(screens.begin());
					return;
				}
			}
		}
	}
}

void GameOver::update(std::vector<std::unique_ptr<Screen>> &screens, sf::Time dt) {

	sf::Vector2f mousePos(sf::Mouse::getPosition().x, sf::Mouse::getPosition().y);
	m_restartButton.checkIfActive(mousePos);
	m_mainMenuButton.checkIfActive(mousePos);
	m_exitButton.checkIfActive(mousePos);

	m_restartButton.update();
	m_mainMenuButton.update();
	m_exitButton.update();

	sf::Sprite canvas_sprite;

	m_canvas->clear(sf::Color(0, 0, 0));
	m_window->clear(sf::Color::Black);

	m_canvas->draw(m_infoText.m_textSprite);
	m_canvas->draw(m_restartButton.m_textSprite);
	m_canvas->draw(m_mainMenuButton.m_textSprite);
	m_canvas->draw(m_exitButton.m_textSprite);
	m_canvas->display();

	canvas_sprite.setTexture(m_canvas->getTexture());
	m_window->draw(canvas_sprite);
	m_window->display();
}

void GameOver::handleKeyboard(sf::Time dt) {
}