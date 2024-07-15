#ifndef __ECS_REGISTRY_H__
#define __ECS_REGISTRY_H__


#ifdef MAX_ENTITIES
#undef MAX_ENTITIES
#endif
#define MAX_ENTITIES 10000

#ifdef MAX_COMPONENT_TYPES
#undef MAX_COMPONENT_TYPES
#endif
#define MAX_COMPONENT_TYPES 32

#ifdef MAX_EVENT_TYPES
#undef MAX_EVENT_TYPES
#endif
#define MAX_EVENT_TYPES 32


#include "ECS.hpp"
#include "SFML/Graphics.hpp"
#include <functional>

namespace ECSRegistry {
	void ECSInitialize();
	void ECSTerminate();
}

// avoid having undefined constructor arguments for events or components,
// as it's more convenient to not have to define every event/component all the time

#pragma region Events
// whenever you create a new type, ensure you register it in the implementation file of this header
namespace EntityEvents {
#pragma region user_defined_events
	struct EventExample final : public Event {
		DUPLICATE_OVERRIDE(EventExample)

		EventExample(uint16_t _var = 0) : var(_var) {};

		uint16_t var;
	};
#pragma endregion user_defined_events
}
#pragma endregion Events
#pragma region Components
// use this file to define new component types.
// whenever you create a new type, ensure you register it in the implementation file of this header
namespace EntityComponents {
#pragma region user_defined_components_section
	struct ComponentExample final : public Component {
		DUPLICATE_OVERRIDE(ComponentExample)

		void system(Entity& entity) final;

		ComponentExample(uint16_t _var = 0) : var(_var) {
			hasSystem = true;
		};

		uint16_t var;
	};
#pragma endregion user_defined_components_section
}
#pragma endregion Components

#endif