#include "InputManager.hpp"
#include "../Key Events/KeyRecorder.hpp"

std::vector<InputEvent> InputManager::inputs{};
std::map<InputName, uint16_t> InputManager::namesToIndexesMap{};

MouseData InputManager::mouseData{};

bool InputManager::hasFocus = true;

bool InputManager::keySetContains(const KeySet& set, const KeyEvent& keyEvent) {
	for (const auto& curSetKeyEvent : set) {
		if (keyEvent == curSetKeyEvent) {
			return true;
		}
	}

	return false;
}

bool InputManager::inputEventIsActive(const InputEvent& inputEvent, const KeySet& keySet) {
	for (KeyEvent curInputKey : inputEvent.linkedKeyEvents) {
		if (keySetContains(keySet, curInputKey)) {
			if (inputEvent.keyLogic == Or) {
				return true;
			}
		}
		else if (inputEvent.keyLogic == And) {
			return false;
		}
	}

	// return whether the keyLogic is equal to And because if it is and, and we made it to the end, then we never found anything false,
	// if it's not And, then it means it's Or and we never found anything true
	return inputEvent.keyLogic == And;
}

void InputManager::inputEventsUpdate() {
	for (InputEvent& curInput : inputs) {
		curInput.isActive = false;
	}

	if (!hasFocus) return;

	KeySet keys = KeyRecorder::keysGet();

	for (InputEvent& curInputEvent : inputs) {
		curInputEvent.isActive = inputEventIsActive(curInputEvent, keys);
	}
}

// registers an InputEvent to inputs
void InputManager::inputRegister(const InputName& name, const KeySet& keys) {
	inputs.push_back(InputEvent(name, keys));
}
// registers an InputEvent to inputs
void InputManager::inputRegister(const InputName& name, const KeySet& keys, const InputKeyLogic keyLogic) {
	inputs.push_back(InputEvent(name, keys, keyLogic));
}

uint16_t InputManager::inputIndexFind(const InputName& name) {
	return namesToIndexesMap[name];
}

