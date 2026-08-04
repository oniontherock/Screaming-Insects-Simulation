#ifndef __CONSOLE_HANDLER_H__
#define __CONSOLE_HANDLER_H__

#include <cstdint>
#include <string>

namespace ConsoleHandler {

	extern std::string gameName;

	void setGameName(std::string name);

	void consoleSetTextColor(uint8_t colorId);
	void consoleResetColor();

	void consolePrintMessage(std::string message);
	void consolePrintColor(std::string message, const uint8_t colorId);
	// prints the specified error message to the console
	void consolePrintErr(std::string message);
	// prints the specified engine progress message to the console
	void consolePrintLoadingEngine(std::string message);
	// prints the specified game progress message to the console
	void consolePrintLoadingGame(std::string message);

	// prints the specified debug message to the console
	void consolePrintDebug(std::string message);
}


#endif