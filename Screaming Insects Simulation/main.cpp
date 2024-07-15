#include "ACECS/ACECS.hpp"

int main() {
	// setup window
	sf::RenderWindow window(sf::VideoMode(1280, 720), "Window");

	Engine::engineInitialize();

	// run main program loop if window is open
	while (window.isOpen()) {

		Engine::engineInputUpdate(window);
		Engine::engineUpdate();

		window.clear(sf::Color::Black);

		// test
		Engine::engineDraw(window);

		window.display();
	}

	return 0;
}