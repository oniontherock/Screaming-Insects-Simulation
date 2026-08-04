#ifndef __SAVE_HANDLER_H__
#define __SAVE_HANDLER_H__

#include "../Auxiliary/ConsoleHandler.hpp"
#include <fstream>
#include <ios>
#include <string>

// class for holding some low level saving related stuff, like the file streams, the save file name,
// and some functions that do the most basic "save" operations, like "fileWrite << obj", and "fileRead >> obj".
class SaveHandler {

	// name of the
	static std::string saveFileName;
	
	static std::ofstream fileWriter;
	static std::ifstream fileReader;

public:
	
	static void saveFileNameSet(std::string nameNew);
	// check if the a file with the name of saveFileName exists
	static bool saveFileExists();

	static void fileWriterOpen();
	static void fileWriterClose();

	static void fileReaderOpen();
	static void fileReaderClose();

	// saves a file without doing any error checking
	template <typename T>
	static void objectSaveIgnoreErrors(T& obj) {
		fileWriter << obj;
	}
	// loads a file without doing any error checking
	template <typename T>
	static void objectLoadIgnoreErrors(T& obj) {
		fileReader >> obj;
	}

	template <typename T>
	static void objectSave(T& obj) {

		try {
			if (!fileWriter.is_open()) {
				throw "fileWriter is not open!";
			}

			fileWriter << obj;
		}
		catch (std::string e) {
			ConsoleHandler::consolePrintErr("Save failed! Reason: " + e);
			return;
		}
	}
	template <typename T>
	static void objectLoad(T& obj) {
		try {
			if (!fileReader.is_open()) {
				throw "fileReader is not open!";
			}

			fileReader >> obj;
		}
		catch (std::string e) {
			ConsoleHandler::consolePrintErr("Load failed! Reason: " + e);
			return;
		}
	}
};

#endif