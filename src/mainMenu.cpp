#include "mainMenu.h"

MainMenu::MainMenu(sf::RenderWindow& window, sf::RenderTexture& canvas, sf::Font& textFont) :
	m_playButton("Play", textFont, sf::Color::Red, sf::Color::White),
	m_exitButton("Exit", textFont, sf::Color::Red, sf::Color::White),
	m_infoButton("Game", textFont, sf::Color::White, sf::Color::White)
{

	m_font = &textFont;
	m_canvas = &canvas;
	m_window = &window;

	m_windowsWidth = sf::VideoMode::getDesktopMode().width;

	m_infoButton.m_textSprite.setCharacterSize(50);
	m_infoButton.setPosition(sf::Vector2f(m_windowsWidth / 2, 100));

	m_playButton.setPosition(sf::Vector2f(m_windowsWidth / 2, 400));
	m_exitButton.setPosition(sf::Vector2f(m_windowsWidth / 2, 500));
}

MainMenu::~MainMenu() {
}

void MainMenu::handleEvents(std::vector<std::unique_ptr<Screen>>& screens, sf::Time dt) {
	sf::Event event;
	while (m_window->pollEvent(event)) {
		if (event.type == sf::Event::Closed) {
			m_window->close();
		}
		if (event.type == sf::Event::KeyPressed) {
			if (event.key.scancode == sf::Keyboard::Scan::Escape) {
				m_window->close();
			}
		}
		if (event.type == sf::Event::MouseButtonPressed) {
			if (event.mouseButton.button == sf::Mouse::Left) {
				if (m_playButton.m_contains) {
					try {
						screens.insert(screens.begin(), std::make_unique<Game>(*m_window, *m_canvas, *m_font));
					}
					catch (std::runtime_error & excp) {
						std::cout << excp.what() << std::endl;
					}
				}
				else if (m_exitButton.m_contains) {
					m_window->close();
				}
			}
		}
	}
}

void MainMenu::handleKeyboard(sf::Time dt) {
}

void MainMenu::update(std::vector<std::unique_ptr<Screen>> &screens, sf::Time dt) {
	
	sf::Vector2f mousePos(sf::Mouse::getPosition().x, sf::Mouse::getPosition().y);
	m_playButton.checkIfActive(mousePos);
	m_exitButton.checkIfActive(mousePos);
	m_playButton.update();
	m_exitButton.update();

	sf::Sprite canvas_sprite;

	m_canvas->clear(sf::Color(0, 0, 0));
	m_window->clear(sf::Color::Black);

	m_canvas->draw(m_infoButton.m_textSprite);
	m_canvas->draw(m_playButton.m_textSprite);
	m_canvas->draw(m_exitButton.m_textSprite);
	m_canvas->display();

	canvas_sprite.setTexture(m_canvas->getTexture());
	m_window->draw(canvas_sprite);
	m_window->display();
}