#ifndef __DUPLICATABLE_H__
#define __DUPLICATABLE_H__

#include <iostream>


struct Duplicatable {
	virtual std::unique_ptr<Duplicatable> duplicate();

	virtual ~Duplicatable() = default;
	// duplicates an object, and returns a unique_ptr converted to T. Note that T must be a derived class of Duplicatable
	// takes a pointer as the object to duplicate
	template <class T>
	static std::unique_ptr<T> duplicateAndConvertToType(Duplicatable* objectToDuplicate) {

		std::unique_ptr<Duplicatable> duplicatablePtr = objectToDuplicate->duplicate();
		// get the raw pointer behind the duplicatablePtr
		Duplicatable* rawPointer = duplicatablePtr.get();
		// release the duplicatablePtr
		duplicatablePtr.release();
		// create a new unique_ptr of type T with the rawPointer we just got
		return std::unique_ptr<T>(static_cast<T*>(rawPointer));
	}
	// duplicates an object, and returns a unique_ptr converted to T. Note that T must be a derived class of Duplicatable
	// takes a reference as the object to duplicate
	template <class T>
	static std::unique_ptr<T> duplicateAndConvertToType(Duplicatable& objectToDuplicate) {

		std::unique_ptr<Duplicatable> duplicatablePtr = objectToDuplicate.duplicate();
		// get the raw pointer behind the duplicatablePtr
		Duplicatable* rawPointer = duplicatablePtr.get();
		// release the duplicatablePtr
		duplicatablePtr.release();
		// create a new unique_ptr of type T with the rawPointer we just got
		return std::unique_ptr<T>(static_cast<T*>(rawPointer));
	}

	static Duplicatable* duplicateAndGetRaw(Duplicatable* objectToDuplicate);
	static Duplicatable* duplicateAndGetRaw(Duplicatable& objectToDuplicate);
};

#endif
