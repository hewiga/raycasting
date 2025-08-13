#include "textButton.h"

TextButton::TextButton(const char* content, sf::Font& font, sf::Color activeColor, sf::Color unactiveColor) {
	m_activeColor = activeColor;
	m_unactiveColor = unactiveColor;

	m_textSprite.setString(content);
	m_textSprite.setCharacterSize(30);
	m_textSprite.setFont(font);
}

void TextButton::setPosition(sf::Vector2f position) {

	m_textSprite.setPosition(sf::Vector2f(
		position.x - m_textSprite.getGlobalBounds().width / 2,
		position.y - m_textSprite.getGlobalBounds().height / 2
	));
}

void TextButton::checkIfActive(sf::Vector2f mousePos) {

	if (m_textSprite.getGlobalBounds().contains(mousePos)) {
		m_contains = 1;
	}
	else {
		m_contains = 0;
	}
}

void TextButton::update() {

	if (m_contains && m_textSprite.getFillColor() == m_unactiveColor) {
		m_textSprite.setFillColor(m_activeColor);
	}
	else if (!m_contains && m_textSprite.getFillColor() == m_activeColor) {
		m_textSprite.setFillColor(m_unactiveColor);
	}
}
