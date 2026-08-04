#ifndef __ECS_H__
#define __ECS_H__

// components
#include "ECS/Components/Component.hpp"
#include "ECS/Components/ComponentIDs.hpp"
#include "ECS/Components/ComponentTemplateManager.hpp"

// entities
#include "ECS/Entities/EntityManager.hpp"
#include "ECS/Entities/Entity.hpp"

// events
#include "ECS/Events/EventIDs.hpp"

// general
#include "ECS/TypeIDAllocator.hpp"
#include "ECS/TypeDefinitions.hpp"

// overrides Duplicatable's duplicate function.
// used when declaring custom events and components to simplify the overriding process
#define DUPLICATE_OVERRIDE(Type) \
	std::unique_ptr<Duplicatable> duplicate() override { \
		return std::unique_ptr<Duplicatable>(new Type()); \
	}

#endif