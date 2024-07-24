#include "../ACECS/ACECS.hpp"
#include "../Include/Common/NumberGenerator.hpp"
#include "../Include/Common/RandomPools.hpp"
#include "../Include/Common/TimeHandler.hpp"

int main() {


	RNGf::initialize();
	poolsInitialize();

	// setup window
	sf::RenderWindow window(sf::VideoMode(1280, 720), "Window");
	//window.setFramerateLimit(60);

	Engine::engineInitialize();

	int fps = 0;
	float fpsTimer = 0;
	int counter = 0;
	int total = 0;

	// run main program loop if window is open
	while (window.isOpen()) {

		TimeHandler::deltaCompute();
		
		Engine::engineInputUpdate(window);
		Engine::engineUpdate();

		window.clear(sf::Color::Black);

		Engine::engineDraw(window);

		window.display();

		fpsTimer += float(TimeHandler::deltaRealGet());
		fps++;

		if (fpsTimer > 1.f) {
			counter++;
			total += fps;

			std::cout << fps << " " << total / counter << " " << counter << std::endl;
			
			fpsTimer = 0.f;
			fps = 0;
		}

	}

	return 0;
}