#include "ConsoleHandler.hpp"

#include <Windows.h>
#include <iostream>

std::string ConsoleHandler::gameName = "Game";

void ConsoleHandler::setGameName(std::string name) {
	gameName = name;
}

void ConsoleHandler::consoleSetTextColor(uint8_t colorId) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colorId);
}
void ConsoleHandler::consoleResetColor() {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
}

void ConsoleHandler::consolePrintMessage(std::string message) {
	std::cout << message << "\n";
}
void ConsoleHandler::consolePrintColor(std::string message, const uint8_t colorId) {
	consoleSetTextColor(colorId);
	consolePrintMessage(message);
	consoleResetColor();
}

void ConsoleHandler::consolePrintErr(std::string message) {
	consolePrintColor("ERROR: " + message, 12);
	consoleResetColor();
}
void ConsoleHandler::consolePrintLoadingEngine(std::string message) {
	consolePrintColor("ACECS > " + message, 2);
	consoleResetColor();
}
void ConsoleHandler::consolePrintLoadingGame(std::string message) {
	consolePrintColor(gameName + " > " + message, 2);
	consoleResetColor();
}

void ConsoleHandler::consolePrintDebug(std::string message) {
	consolePrintColor(message, 9);
	consoleResetColor();
}
