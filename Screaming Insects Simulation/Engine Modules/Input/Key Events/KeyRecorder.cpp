#include "KeyRecorder.hpp"

// set of keys from the previous frame
std::unordered_set<KeyName> KeyRecorder::keyNamesPrev{};
// set of keys from the current frame
std::unordered_set<KeyName> KeyRecorder::keyNames{};
std::set<KeyName> KeyRecorder::registeredKeyNames{};

float KeyRecorder::mouseWheelMoveAmount{};

KeySet KeyRecorder::keyEvents{};

std::unordered_map<KeyName, sf::Keyboard::Key> KeyRecorder::nameToKeyMap{
    { "Unknown", sf::Keyboard::Key::Unknown },
    { "A", sf::Keyboard::Key::A },
    { "B", sf::Keyboard::Key::B },
    { "C", sf::Keyboard::Key::C },
    { "D", sf::Keyboard::Key::D },
    { "E", sf::Keyboard::Key::E },
    { "F", sf::Keyboard::Key::F },
    { "G", sf::Keyboard::Key::G },
    { "H", sf::Keyboard::Key::H },
    { "I", sf::Keyboard::Key::I },
    { "J", sf::Keyboard::Key::J },
    { "K", sf::Keyboard::Key::K },
    { "L", sf::Keyboard::Key::L },
    { "M", sf::Keyboard::Key::M },
    { "N", sf::Keyboard::Key::N },
    { "O", sf::Keyboard::Key::O },
    { "P", sf::Keyboard::Key::P },
    { "Q", sf::Keyboard::Key::Q },
    { "R", sf::Keyboard::Key::R },
    { "S", sf::Keyboard::Key::S },
    { "T", sf::Keyboard::Key::T },
    { "U", sf::Keyboard::Key::U },
    { "V", sf::Keyboard::Key::V },
    { "W", sf::Keyboard::Key::W },
    { "X", sf::Keyboard::Key::X },
    { "Y", sf::Keyboard::Key::Y },
    { "Z", sf::Keyboard::Key::Z },
    { "Num0", sf::Keyboard::Key::Num0 },
    { "Num1", sf::Keyboard::Key::Num1 },
    { "Num2", sf::Keyboard::Key::Num2 },
    { "Num3", sf::Keyboard::Key::Num3 },
    { "Num4", sf::Keyboard::Key::Num4 },
    { "Num5", sf::Keyboard::Key::Num5 },
    { "Num6", sf::Keyboard::Key::Num6 },
    { "Num7", sf::Keyboard::Key::Num7 },
    { "Num8", sf::Keyboard::Key::Num8 },
    { "Num9", sf::Keyboard::Key::Num9 },
    { "Escape", sf::Keyboard::Key::Escape },
    { "LControl", sf::Keyboard::Key::LControl },
    { "LShift", sf::Keyboard::Key::LShift },
    { "LAlt", sf::Keyboard::Key::LAlt },
    { "LSystem", sf::Keyboard::Key::LSystem },
    { "RControl", sf::Keyboard::Key::RControl },
    { "RShift", sf::Keyboard::Key::RShift },
    { "RAlt", sf::Keyboard::Key::RAlt },
    { "RSystem", sf::Keyboard::Key::RSystem },
    { "Menu", sf::Keyboard::Key::Menu },
    { "LBracket", sf::Keyboard::Key::LBracket },
    { "RBracket", sf::Keyboard::Key::RBracket },
    { "Semicolon", sf::Keyboard::Key::Semicolon },
    { "Comma", sf::Keyboard::Key::Comma },
    { "Period", sf::Keyboard::Key::Period },
    { "Apostrophe", sf::Keyboard::Key::Apostrophe },
    { "Slash", sf::Keyboard::Key::Slash },
    { "Backslash", sf::Keyboard::Key::Backslash },
    { "Grave", sf::Keyboard::Key::Grave },
    { "Equal", sf::Keyboard::Key::Equal },
    { "Hyphen", sf::Keyboard::Key::Hyphen },
    { "Space", sf::Keyboard::Key::Space },
    { "Enter", sf::Keyboard::Key::Enter },
    { "Backspace", sf::Keyboard::Key::Backspace },
    { "Tab", sf::Keyboard::Key::Tab },
    { "PageUp", sf::Keyboard::Key::PageUp },
    { "PageDown", sf::Keyboard::Key::PageDown },
    { "End", sf::Keyboard::Key::End },
    { "Home", sf::Keyboard::Key::Home },
    { "Insert", sf::Keyboard::Key::Insert },
    { "Delete", sf::Keyboard::Key::Delete },
    { "Add", sf::Keyboard::Key::Add },
    { "Subtract", sf::Keyboard::Key::Subtract },
    { "Multiply", sf::Keyboard::Key::Multiply },
    { "Divide", sf::Keyboard::Key::Divide },
    { "Left", sf::Keyboard::Key::Left },
    { "Right", sf::Keyboard::Key::Right },
    { "Up", sf::Keyboard::Key::Up },
    { "Down", sf::Keyboard::Key::Down },
    { "Numpad0", sf::Keyboard::Key::Numpad0 },
    { "Numpad1", sf::Keyboard::Key::Numpad1 },
    { "Numpad2", sf::Keyboard::Key::Numpad2 },
    { "Numpad3", sf::Keyboard::Key::Numpad3 },
    { "Numpad4", sf::Keyboard::Key::Numpad4 },
    { "Numpad5", sf::Keyboard::Key::Numpad5 },
    { "Numpad6", sf::Keyboard::Key::Numpad6 },
    { "Numpad7", sf::Keyboard::Key::Numpad7 },
    { "Numpad8", sf::Keyboard::Key::Numpad8 },
    { "Numpad9", sf::Keyboard::Key::Numpad9 },
    { "F1", sf::Keyboard::Key::F1 },
    { "F2", sf::Keyboard::Key::F2 },
    { "F3", sf::Keyboard::Key::F3 },
    { "F4", sf::Keyboard::Key::F4 },
    { "F5", sf::Keyboard::Key::F5 },
    { "F6", sf::Keyboard::Key::F6 },
    { "F7", sf::Keyboard::Key::F7 },
    { "F8", sf::Keyboard::Key::F8 },
    { "F9", sf::Keyboard::Key::F9 },
    { "F10", sf::Keyboard::Key::F10 },
    { "F11", sf::Keyboard::Key::F11 },
    { "F12", sf::Keyboard::Key::F12 },
    { "F13", sf::Keyboard::Key::F13 },
    { "F14", sf::Keyboard::Key::F14 },
    { "F15", sf::Keyboard::Key::F15 },
    { "Pause", sf::Keyboard::Key::Pause },
};
std::unordered_map<sf::Keyboard::Key, KeyName> KeyRecorder::keyToNameMap{};


std::unordered_map<KeyName, sf::Mouse::Button> KeyRecorder::nameToButtonMap{
    { "Mouse Left", sf::Mouse::Button::Left },
    { "Mouse Middle", sf::Mouse::Button::Middle },
    { "Mouse Right", sf::Mouse::Button::Right },
};
std::unordered_map<sf::Mouse::Button, KeyName> KeyRecorder::buttonToNameMap{};

void KeyRecorder::buttonToNameMapInitialize() {
    for (const auto& nameButtonPair : nameToButtonMap) {
        buttonToNameMap.insert({ nameButtonPair.second, nameButtonPair.first });
    }

}

void KeyRecorder::keyToNameMapInitialize() {
    for (const auto& nameKeyPair : nameToKeyMap) {
        keyToNameMap.insert({ nameKeyPair.second, nameKeyPair.first });
    }
}

// sets an InputEvent's transitionType,
KeyEvent KeyRecorder::keyEventGet(const KeyName& key) {

    bool activePrev = keyNamesPrev.count(key);
    bool activeNow = keyNames.count(key);

    // we get the keyTransition by getting the difference in key presses between now and last frame
    // this works because:
    // 0 - 1 = -1|Released
    // 1 - 1 = 0|Held
    // 1 - 0 = 1|Pressed
    uint8_t keyTransition = (activeNow - activePrev) + 1;

    return KeyEvent(key, static_cast<KeyTransition>(keyTransition));
}

bool KeyRecorder::keyboardIsKeyNameActive(KeyName name) {
    return sf::Keyboard::isKeyPressed(nameToKeyMap[name]);
}
bool KeyRecorder::mouseIsKeyNameActive(KeyName name) {
    return sf::Mouse::isButtonPressed(nameToButtonMap[name]);
}


// records every input in "event"
void KeyRecorder::keysUpdate() {

    keyEvents.clear();

    keyNamesPrev = keyNames;
    keyNames.clear();

    for (const KeyName& curRegisteredKeyName : registeredKeyNames) {

        bool keyNameIsActive = false;

        if (nameToKeyMap.count(curRegisteredKeyName) > 0) {
            keyNameIsActive = keyboardIsKeyNameActive(curRegisteredKeyName);
        }
        else {
            keyNameIsActive = mouseIsKeyNameActive(curRegisteredKeyName);
        }

        if (keyNameIsActive) {
            keyNames.insert(curRegisteredKeyName);;
        }
    }

    std::unordered_set<KeyName> inputsAll = keyNames;
    inputsAll.insert(keyNamesPrev.begin(), keyNamesPrev.end());

    for (const KeyName& curInput : inputsAll) {
        keyEvents.insert(keyEventGet(curInput));
    }
}

void KeyRecorder::keyNameRegister(const KeyName& name) {
    registeredKeyNames.insert(name);
}

void KeyRecorder::keySetRegister(const KeySet& keySet) {
    for (const KeyEvent& curKeyEvent : keySet) {
        registeredKeyNames.insert(curKeyEvent.keyName);
    }
}

const KeySet& KeyRecorder::keysGet() {
    return keyEvents;
}
