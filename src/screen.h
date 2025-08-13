#pragma once
#include <SFML/Graphics.hpp>

class Screen {
public:
	virtual ~Screen() {};
	virtual void update(std::vector<std::unique_ptr<Screen>> &screens, sf::Time dt) = 0;
	virtual void handleKeyboard(sf::Time dt) = 0;
	virtual void handleEvents(std::vector<std::unique_ptr<Screen>> &screens, sf::Time dt) = 0;
};