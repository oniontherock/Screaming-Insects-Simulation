#ifndef __COMPONENT_IDS_H__
#define __COMPONENT_IDS_H__

#include "Component.hpp"
#include <cstdint>
#include <vector>

namespace EntityComponents {

	// an ID representing a certain TYPE of component, NOT a specific component instance
	typedef uint16_t ComponentTypeID;

	// the maximum value for any ID
	inline ComponentTypeID maxID;
	// the total amount of components
	inline ComponentTypeID totalComponents;
	// a vector of instances of every type of component, sorted by component type. Assigned when the component's Id is registered.
	inline std::vector<ComponentUniquePtr> componentsAll;

	// ComponentTypesHolder is a sort of helper struct that holds the ComponentTypeID of every type of component,
	// an ID is simply an integer assigned uniquely to every child of base class "Component",
	// the way you get the ID of a component type is like this: "ComponentIDHolder<TYPE>::ID".
	template <class T>
	struct ComponentIDs {
		static ComponentTypeID ID;

		static void IDSet(ComponentTypeID value) {
			ID = value;

			if (ID > maxID) maxID = ID;

			if (ID + 1 > totalComponents) totalComponents = ID + 1;

			componentsAll.push_back(ComponentUniquePtr(new T()));
		}
	};

	// define ComponentIDHolder's ID variable as 0 for every type.
	// note that the ID for every type is assigned later, 
	// if you wish to see how this is done, see the "ComponentRegistry.cpp" file, or the "TypeAllocator.hpp" file.

	// the ID of a component type, used for accessing certain types of components in arrays, among other things
	template <class T>
	ComponentTypeID ComponentIDs<T>::ID{ 0 };

	// a pair of ComponentTypeID and Component instance,
	// should be used when knowing the ID of a component is necessary, but knowing the type of component is not possible.
	// note that this type contains a shared_ptr, because certain usages are impossible if it is a unique_ptr, like list initialization
	typedef std::pair<EntityComponents::ComponentTypeID, ComponentSharedPtr> ComponentIndexInstancePair;

	// creates a new ComponentIndexInstancePair from the type T with the arguments "args"
	template <class T, typename... Args>
	ComponentIndexInstancePair createComponentPairFromType(Args... args) {
		return ComponentIndexInstancePair(ComponentIDs<T>::ID, ComponentSharedPtr(new T(args...)));
	}
};

#endif