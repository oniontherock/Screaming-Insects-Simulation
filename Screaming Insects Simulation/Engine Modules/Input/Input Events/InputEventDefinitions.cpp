#include "InputEventDefinitions.hpp"


InputEvent::InputEvent(InputName _name, KeySet keyEvents) : name(_name), linkedKeyEvents(keyEvents) {};
InputEvent::InputEvent(InputName _name, KeySet keyEvents, InputKeyLogic _keyLogic) : name(_name), linkedKeyEvents(keyEvents), keyLogic(_keyLogic) {};
