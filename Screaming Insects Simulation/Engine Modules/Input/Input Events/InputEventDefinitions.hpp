#ifndef __INPUT_EVENT_DEFINITIONS_H__
#define __INPUT_EVENT_DEFINITIONS_H__

#include "../Key Events/KeyEventDefinitions.hpp"

typedef const char* InputName;

// enum for the behavior of KeySets inside of InputEvents.
enum InputKeyLogic : uint16_t {
	Or, // default behavior, if any button is pressed, the InputEvent is set to active,
	And, // all keys must be active for the InputEvent to be active,
};

struct InputEvent {
	InputEvent(InputName _name, KeySet keyEvents);
	InputEvent(InputName _name, KeySet keyEvents, InputKeyLogic _keyLogic);

	InputName name = "Blank";
	KeySet linkedKeyEvents;

	InputKeyLogic keyLogic = InputKeyLogic::Or;
	bool isActive = false;
};

#endif