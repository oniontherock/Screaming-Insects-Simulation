#include "Duplicatable.hpp"

#include "../../Auxiliary/ConsoleHandler.hpp"


std::unique_ptr<Duplicatable> Duplicatable::duplicate() {
	ConsoleHandler::consolePrintErr("duplicate called on base " + std::string("Duplicatable ") + "class, downcasting from this duplicate may fail.");
	return std::unique_ptr<Duplicatable>(new Duplicatable());
}

Duplicatable* Duplicatable::duplicateAndGetRaw(Duplicatable* objectToDuplicate) {
	std::unique_ptr<Duplicatable> duplicatablePtr = objectToDuplicate->duplicate();

	Duplicatable* rawPointer = duplicatablePtr.get();

	duplicatablePtr.release();

	return rawPointer;
}

Duplicatable* Duplicatable::duplicateAndGetRaw(Duplicatable& objectToDuplicate) {
	std::unique_ptr<Duplicatable> duplicatablePtr = objectToDuplicate.duplicate();

	Duplicatable* rawPointer = duplicatablePtr.get();

	duplicatablePtr.release();

	return rawPointer;
}