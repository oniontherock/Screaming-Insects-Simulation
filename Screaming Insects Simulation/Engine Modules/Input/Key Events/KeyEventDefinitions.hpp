#ifndef __KEY_EVENT_DEFINITIONS_H__
#define __KEY_EVENT_DEFINITIONS_H__

#include <cstdint>
#include <set>

enum KeyTransition : uint8_t {
    Released,
    Held,
    Pressed,
};

typedef const char* KeyName;

struct KeyEvent {

    KeyEvent(KeyName _keyName, KeyTransition _transitionType) : keyName(_keyName), transitionType(_transitionType) {};

    KeyName keyName;

    KeyTransition transitionType;

    bool operator< (const KeyEvent& other) const {
        return keyName < other.keyName;
    }
    bool operator== (const KeyEvent& other) const {
        return (keyName == other.keyName) && (transitionType == other.transitionType);
    }
};

typedef std::set<KeyEvent> KeySet;

#endif