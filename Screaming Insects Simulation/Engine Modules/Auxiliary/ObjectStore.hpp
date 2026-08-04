#ifndef __OBJECT_STORE_H__
#define __OBJECT_STORE_H__

#include "ConsoleHandler.hpp"
#include <string>
#include <unordered_map>

// stores a bunch of objects accessed by name, used for stuff like sf::Texture
template <typename T>
class ObjectStore {
	// keeps a a bunch objects, accessed by name
	std::unordered_map<std::string, T> objectsUMap;

public:

	// adds a new blank object to the objectsUMap with the given name
	void objectInitialize(std::string name) {
		objectsUMap.insert({ name, T() });
	}
	// adds a new blank object to the objectsUMap with the given name
	void objectInitialize(const char* name) {
		objectInitialize(std::string(name));
	}

	// adds a new blank object to the objectsUMap with the given name, and returns a reference to the new object
	T& objectInitializeAndGet(std::string name) {
		objectInitialize(name);
		return objectGet(name);
	}
	// adds a new blank object to the objectsUMap with the given name, and returns a reference to the new object
	T& objectInitializeAndGet(const char* name) {
		return objectInitializeAndGet(std::string(name));
	}

	// returns a reference to the given object
	T& objectGet(std::string objectName) {
		return objectsUMap[objectName];
	}
	// returns a reference to the given object
	T& objectGet(const char* objectName) {
		return objectGet(std::string(objectName));
	}

	// returns whether the given object existsd
	bool objectExists(std::string objectName) {
		return objectsUMap.contains(objectName);
	}
	// returns a reference to the given object
	bool objectExists(const char* objectName) {
		return objectExists(std::string(objectName));
	}


	// adds a object to the objectsUMap by copying the objectInstance, assigns it the specified name
	void objectAddFromReference(std::string name, T& objectReference) {
		objectsUMap.insert({ name, objectReference });
	}
	// adds a object to the objectsUMap by copying the objectInstance, assigns it the specified name
	void objectAddFromReference(const char* name, T& objectReference) {
		objectAddFromInstance(std::string(name), objectReference);
	}
	// adds a object to the objectsUMap by copying the objectInstance, assigns it the specified name
	void objectAddFromInstance(std::string name, T objectInstance) {
		objectsUMap.insert({ name, objectInstance });
	}
	// adds a object to the objectsUMap by copying the objectInstance, assigns it the specified name
	void objectAddFromInstance(const char* name, T objectInstance) {
		objectAddFromInstance(std::string(name), objectInstance);
	}
};

#endif