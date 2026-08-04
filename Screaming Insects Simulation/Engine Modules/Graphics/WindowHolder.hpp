#ifndef __WINDOW_HOLDER_H__
#define __WINDOW_HOLDER_H__

#include "SFML/Graphics.hpp"

class WindowHolder {
	static sf::RenderWindow window;

public:

	static void windowInitialize(sf::VideoMode videoMode, std::string name);
	static void windowInitialize(sf::Vector2f size, std::string name);
	static void windowInitialize(sf::Vector2f size);
	
	static sf::RenderWindow& windowGet();
};

#endif