#pragma once
#include <SFML/Graphics.hpp>

class TextButton {
	sf::Color m_unactiveColor;
	sf::Color m_activeColor;
	sf::Vector2f m_position;

public:
	sf::Text m_textSprite;
	bool m_contains;

	TextButton(const char* content, sf::Font& font, sf::Color activeColor, sf::Color unactiveColor);
	void setPosition(sf::Vector2f position);
	void checkIfActive(sf::Vector2f mousePos);
	void update();
};