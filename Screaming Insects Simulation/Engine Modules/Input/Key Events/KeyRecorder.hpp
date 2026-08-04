#ifndef __KEY_RECORDER_H__
#define __KEY_RECORDER_H__

#include "KeyEventDefinitions.hpp"
#include "SFML/Window/Event.hpp"
#include <set>
#include <unordered_map>
#include <unordered_set>


// records input and assigns it a transition type
namespace KeyRecorder {

    // set of keys from the previous frame
    extern std::unordered_set<KeyName> keyNamesPrev;
    // set of keys from the current frame
    extern std::unordered_set<KeyName> keyNames;
    extern std::set<KeyName> registeredKeyNames;

    extern float mouseWheelMoveAmount;

    extern KeySet keyEvents;

    extern std::unordered_map<KeyName, sf::Keyboard::Key> nameToKeyMap;
    extern std::unordered_map<sf::Keyboard::Key, KeyName> keyToNameMap;


    extern std::unordered_map<KeyName, sf::Mouse::Button> nameToButtonMap;
    extern std::unordered_map<sf::Mouse::Button, KeyName> buttonToNameMap;

    void buttonToNameMapInitialize();
    void keyToNameMapInitialize();
    // sets an InputEvent's transitionType,
    KeyEvent keyEventGet(const KeyName& key);
    bool keyboardIsKeyNameActive(KeyName name);
    bool mouseIsKeyNameActive(KeyName name);
    // records every input in "event"
    void keysUpdate();
    void keyNameRegister(const KeyName& name);
    void keySetRegister(const KeySet& keySet);
    const KeySet& keysGet();
}


#endif