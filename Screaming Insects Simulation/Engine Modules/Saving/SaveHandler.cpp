#include "SaveHandler.hpp"

std::string SaveHandler::saveFileName = "save.dat";

std::ofstream SaveHandler::fileWriter{};
std::ifstream SaveHandler::fileReader{};

void SaveHandler::saveFileNameSet(std::string nameNew) {

	try {
		if (fileWriter.is_open()) {
			throw "Writer";
		}
		if (fileReader.is_open()) {
			throw "Reader";
		}

		saveFileName = nameNew;
	}
	catch (std::string e) {
		ConsoleHandler::consolePrintErr("Save file name changed while file" + e + " is open! Please close before changing save file");
		return;
	}
}
bool SaveHandler::saveFileExists() {
	return std::ifstream(saveFileName).good();
}

void SaveHandler::fileWriterOpen() {
	fileWriter.open(saveFileName, std::ios::out | std::ios::binary);

	if (!fileWriter) {
		ConsoleHandler::consolePrintErr("Save file failed to open");
	}
}
void SaveHandler::fileWriterClose() {
	fileWriter.close();
}

void SaveHandler::fileReaderOpen() {
	fileReader.open(saveFileName, std::ios::in | std::ios::binary);

	if (!fileWriter) {
		ConsoleHandler::consolePrintErr("Save file failed to open");
	}
}
void SaveHandler::fileReaderClose() {
	fileReader.close();
}
