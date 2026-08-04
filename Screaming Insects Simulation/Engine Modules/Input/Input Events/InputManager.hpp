#ifndef __INPUT_MANAGER_H__
#define __INPUT_MANAGER_H__

#include "InputEventDefinitions.hpp"
#include "SFML/System/Vector2.hpp"
#include <map>
#include <vector>


struct MouseData {
	sf::Vector2i position;
	float scrollAmount = 0.f;
};

namespace InputManager {

	extern std::vector<InputEvent> inputs;
	extern std::map<InputName, uint16_t> namesToIndexesMap;
	
	extern MouseData mouseData;

	extern bool hasFocus;

	bool keySetContains(const KeySet& set, const KeyEvent& keyEvent);

	bool inputEventIsActive(const InputEvent& inputEvent, const KeySet& keySet);

	void inputEventsUpdate();

	// registers an InputEvent to inputs
	void inputRegister(const InputName& name, const KeySet& keys);
	// registers an InputEvent to inputs
	void inputRegister(const InputName& name, const KeySet& keys, const InputKeyLogic keyLogic);

	uint16_t inputIndexFind(const InputName& name);
}



#endif