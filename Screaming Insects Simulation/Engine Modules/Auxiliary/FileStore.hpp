#ifndef __FILE_STORE_H__
#define __FILE_STORE_H__

#include "ConsoleHandler.hpp"
#include "ObjectStore.hpp"
#include <string>
#include <unordered_map>

// stores the results of various file loading operations, like images or sounds
template <typename T>
class FileStore : public ObjectStore<T> {
	// the default file extension for file loading
	std::string extensionDefault = "Err";
	// keeps a a bunch sf::files, accessed by name
	std::unordered_map<std::string, T> objectsUMap;
protected:
	// sets the extensionDefault
	void extensionDefaultSet(std::string extension) {
		extensionDefault = extension;
	}
public:
	// gets the extensionDefault
	std::string extensionDefaultGet() {
		return extensionDefault;
	}

	// initializes and loads a file from the given file name and extension
	void fileLoadFromName(std::string fileName, std::string extensionName) {

		T& file = ObjectStore<T>::objectInitializeAndGet(fileName);

		try {
			if (extensionName == "err") {
				if (extensionDefault == "err") {
					throw std::string("Default extension invalid");
				}
				else {
					throw std::string("Extension invalid");
				}
			}
			if (!file.loadFromFile(fileName + "." + extensionName)) {
				throw std::string("Unknown");
			}
		}
		catch (std::string e) {
			ConsoleHandler::consolePrintErr("File loading failed on file: \"" + fileName + "." + extensionName + "\"" + ", Reason: " + e);
		}
	}
	// initializes and loads a file from the given file name, extension defaults to defaultExtension
	void fileLoadFromName(std::string fileName) {
		fileLoadFromName(fileName, extensionDefault);
	}
	// initializes and loads a file from the given file name and extension
	void fileLoadFromName(const char* fileName, const char* extensionName) {
		fileLoadFromName(std::string(fileName), std::string(extensionName));
	}
	// initializes and loads a file from the given file name, extension defaults to defaultExtension
	void fileLoadFromName(const char* fileName) {
		fileLoadFromName(std::string(fileName));
	}

	// initializes and loads a file from the given file name and extension, then returns it.
	T& fileLoadFromNameAndGet(std::string fileName, std::string extensionName) {
		fileLoadFromName(fileName, extensionName);
		return ObjectStore<T>::objectGet(fileName);
	}
	// initializes and loads a file from the given file name, then returns it, extension defaults to defaultExtension
	T& fileLoadFromNameAndGet(std::string fileName) {
		return fileLoadFromNameAndGet(fileName, extensionDefault);
	}

	// initializes and loads a file from the given file name and extension, then returns it.
	T& fileLoadFromNameAndGet(const char* fileName, const char* extensionName) {
		return fileLoadFromNameAndGet(std::string(fileName), std::string(extensionName));
	}
	// initializes and loads a file from the given file name, then returns it, extension defaults to defaultExtension
	T& fileLoadFromNameAndGet(const char* fileName) {
		return fileLoadFromNameAndGet(std::string(fileName));
	}


	// returns a file if it exists, or loads it from the given fileName and extensionDefault, then returns it.
	T& fileGetOrLoadFromName(std::string fileName, std::string extensionName) {
		if (!ObjectStore<T>::objectExists(fileName)) {
			fileLoadFromName(fileName, extensionName);
		}
		return ObjectStore<T>::objectGet(fileName);
	}
	// returns a file if it exists, or loads it from the given fileName and extensionName, then returns it.
	T& fileGetOrLoadFromName(std::string fileName) {
		return fileGetOrLoadFromName(fileName, extensionDefault);
	}

	// returns a file if it exists, or loads it from the given fileName and extensionName, then returns it.
	T& fileGetOrLoadFromName(const char* fileName, const char* extensionName) {
		return fileGetOrLoadFromName(std::string(fileName), std::string(extensionName));
	}
	// returns a file if it exists, or loads it from the given fileName and extensionDefault, then returns it.
	T& fileGetOrLoadFromName(const char* fileName) {
		return fileGetOrLoadFromName(std::string(fileName));
	}
};

#endif