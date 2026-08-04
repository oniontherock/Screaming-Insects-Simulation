#include "InputInterface.hpp"

#include "../Auxiliary/ConsoleHandler.hpp"
#include "Input Events/InputManager.hpp"
#include "Key Events/KeyRecorder.hpp"


bool InputInterface::inputGetActive(const InputName& name) {

	try {

		const uint16_t& indexFromName = InputManager::inputIndexFind(name);

		if (indexFromName < InputManager::inputs.size()) {
			return InputManager::inputs[indexFromName].isActive;
		}
		else {
			throw "Invalid input name: \"" + std::string(name) + std::string("\"");
		}
	}
	catch (const char* message) {
		ConsoleHandler::consolePrintErr(message);
		return false;
	}
}

sf::Vector2i InputInterface::windowMousePositionGet() {
	return InputManager::mouseData.position;
}
float InputInterface::mouseScrollAmountGet() {
	return InputManager::mouseData.scrollAmount;
}

void InputInterface::mousePositionUpdate(sf::Vector2i position) {
	InputManager::mouseData.position = position;
}
void InputInterface::mousePositionUpdate(uint16_t x, uint16_t y) {
	mousePositionUpdate(sf::Vector2i(x, y));
}
void InputInterface::mouseScrollAmountUpdate(float scrollAmount) {
	InputManager::mouseData.scrollAmount = scrollAmount;
}

bool InputInterface::focusGet() {
	return InputManager::hasFocus;
}


void InputInterface::inputRegister(const InputName& name, const KeySet& keys) {
	KeyRecorder::keySetRegister(keys);
	InputManager::namesToIndexesMap.insert({ name, uint16_t(InputManager::inputs.size()) });
	InputManager::inputs.push_back(InputEvent(name, keys));
}

void InputInterface::inputRegister(const InputName& name, const KeySet& keys, InputKeyLogic keyLogic) {
	KeyRecorder::keySetRegister(keys);
	InputManager::namesToIndexesMap.insert({ name, uint16_t(InputManager::inputs.size()) });
	InputManager::inputs.push_back(InputEvent(name, keys, keyLogic));
}

void InputInterface::inputUpdate() {
	KeyRecorder::keysUpdate();
	InputManager::inputEventsUpdate();
}

void InputInterface::eventsProcess(sf::RenderWindow& window) {

	mouseScrollAmountUpdate(0);

	while (const std::optional event = window.pollEvent()) {
		if (event->is<sf::Event::Closed>()) {
			window.close();
			break;
		}
		else if (const auto* mouseMove = event->getIf<sf::Event::MouseMoved>()) {
			mousePositionUpdate(sf::Vector2i(window.mapPixelToCoords(mouseMove->position)));
			break;
		}
		else if (const auto* mouseWheelScrol = event->getIf<sf::Event::MouseWheelScrolled>()) {
			mouseScrollAmountUpdate(mouseWheelScrol->delta);
			break;
		}
		else if (event->is<sf::Event::FocusGained>()) {
			InputManager::hasFocus = true;
			break;
		}
		else if (event->is<sf::Event::FocusLost>()) {
			InputManager::hasFocus = false;
			break;

		}
	}
}
