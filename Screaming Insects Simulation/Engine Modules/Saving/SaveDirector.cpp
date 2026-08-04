#include "SaveDirector.hpp"

void SaveDirector::saveBegin() {
	SaveHandler::fileWriterOpen();
}
void SaveDirector::saveEnd() {
	SaveHandler::fileWriterClose();
}


void SaveDirector::loadBegin() {
	SaveHandler::fileReaderOpen();
}
void SaveDirector::loadEnd() {
	SaveHandler::fileReaderClose();
}

