#ifndef __SAVE_DIRECTOR_H__
#define __SAVE_DIRECTOR_H__

#include "SaveHandler.hpp"

// handles the actual saving/loading process, I.E. controls which things are saved/loaded first.
struct SaveDirector {

	// begins the saving process
	static void saveBegin();
	// saves the game, defined in the "SaveFunctions.cpp" file of the user module
	static void gameDataSave();
	// ends the saving process
	static void saveEnd();

	// begins the loading process
	static void loadBegin();
	// loads the game, defined in the "SaveFunctions.cpp" file of the user module
	static void gameDataLoad();
	// ends the loading process
	static void loadEnd();

};

#endif
