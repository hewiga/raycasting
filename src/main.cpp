#include <SFML/Graphics.hpp>
#include "screen.h"
#include "mainMenu.h"


int main() {
    _CrtDumpMemoryLeaks();
    sf::Font textFont;
    if (!textFont.loadFromFile("assets\\fonts\\Roboto-Regular.ttf")) {
        return -1;
    }

    //prepare window
    sf::RenderWindow window(
        sf::VideoMode(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height),
        "Game", 
        sf::Style::Fullscreen
    );
    window.setFramerateLimit(60);

    //prepare canvas
    sf::RenderTexture canvas;
    sf::Sprite canvas_sprite;
    canvas.create(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height);

    std::vector<std::unique_ptr<Screen>> screens;
    screens.push_back(std::make_unique<MainMenu>(window, canvas, textFont));

    sf::Clock deltaClock;
    sf::Time dt = deltaClock.restart();

    while (window.isOpen()) {

        screens[0]->handleEvents(screens, dt);
        screens[0]->handleKeyboard(dt);
        screens[0]->update(screens, dt);

        dt = deltaClock.restart();
    }
    
	return 0;
}