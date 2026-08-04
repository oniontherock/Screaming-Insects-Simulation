#include "WindowHolder.hpp"

sf::RenderWindow WindowHolder::window{};

void WindowHolder::windowInitialize(sf::VideoMode videoMode, std::string name) {
	window.create(videoMode, name);
}
void WindowHolder::windowInitialize(sf::Vector2f size, std::string name) {
	windowInitialize(sf::VideoMode(sf::Vector2u(uint32_t(size.x), uint32_t(size.y))), name);
}
void WindowHolder::windowInitialize(sf::Vector2f size) {
	windowInitialize(size, "Window");
}

sf::RenderWindow& WindowHolder::windowGet() {
	return window;
}

