#include "../ACECS/ACECS.hpp"
#include "../Include/Common/NumberGenerator.hpp"

int main() {
	
	RNGf::initialize();

	// setup window
	sf::RenderWindow window(sf::VideoMode(1280, 720), "Window");
	window.setFramerateLimit(60);

	Engine::engineInitialize();

	// run main program loop if window is open
	while (window.isOpen()) {

		Engine::engineInputUpdate(window);
		Engine::engineUpdate();

		window.clear(sf::Color::Black);

		Engine::engineDraw(window);

		window.display();
	}

	return 0;
}