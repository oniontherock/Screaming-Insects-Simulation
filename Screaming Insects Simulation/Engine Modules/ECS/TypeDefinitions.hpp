#pragma once

#include <cstdint>


enum class EntityUpdateType : uint8_t {
	Never, // the entity is never updated by the entity updater, the entity can still be updated by external sources, the entity is also updated when first created.
	Frame, // the entity is updated every frame by the entity updater.
	Observation, // the entity is only updated when an entity marked as an observer is within the observer's observation distance to the entity.
};

typedef uint32_t EntityId;

extern uint32_t MAX_ENTITIES;
extern uint16_t MAX_COMPONENT_TYPES;
extern uint16_t MAX_EVENT_TYPES;
