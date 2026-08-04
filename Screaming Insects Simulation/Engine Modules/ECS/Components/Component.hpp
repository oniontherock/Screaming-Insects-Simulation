#ifndef __COMPONENT_H__
#define __COMPONENT_H__

#include "../Duplicatable/Duplicatable.hpp"
#include <fstream>

struct Entity;

namespace EntityComponents {

	// base component class
	struct Component : public Duplicatable{

		virtual ~Component() = default;

		virtual void system(Entity&) {}

		// saves the data of the component
		virtual void save(std::ofstream&) {}
		// loads the data of the component
		virtual void load(std::ifstream&) {}

		bool hasSystem = false;
	};

	// used to initialize component IDs, defined in the ECSRegistry.cpp file
	void componentIDsInitialize();
	void componentTemplatesInitialize();
};

typedef std::unique_ptr<EntityComponents::Component> ComponentUniquePtr;
typedef std::shared_ptr<EntityComponents::Component> ComponentSharedPtr;

#endif