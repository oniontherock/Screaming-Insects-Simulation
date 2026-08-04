#ifndef __INPUT_INTERFACE_H__
#define __INPUT_INTERFACE_H__

#include "SFML/Graphics.hpp"
#include "Input Events/InputEventDefinitions.hpp"
#include "Key Events/KeyEventDefinitions.hpp"

namespace InputInterface {


	bool inputGetActive(const InputName& name);

	sf::Vector2i windowMousePositionGet();
	float mouseScrollAmountGet();

	void mouseScrollAmountUpdate(float scrollAmount);
	void mousePositionUpdate(sf::Vector2i position);
	void mousePositionUpdate(uint16_t x, uint16_t y);
	
	bool focusGet();

	// registers an InputEvent to inputs
	void inputRegister(const InputName& name, const KeySet& keys);
	// registers an InputEvent to inputs
	void inputRegister(const InputName& name, const KeySet& keys, InputKeyLogic keyLogic);

	void inputUpdate();

	void eventsProcess(sf::RenderWindow& window);
}

#endif