#ifndef __ECS_REGISTRY_H__
#define __ECS_REGISTRY_H__


#ifdef MAX_ENTITIES
#undef MAX_ENTITIES
#endif
#define MAX_ENTITIES 15000

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
#include "../Include/Simulation/TargetTypes.hpp"
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
	struct EventScream final : public Event {
		std::unique_ptr<Duplicatable> duplicate() override {
			return std::unique_ptr<Duplicatable>(new EventScream(angleToScream, info));
		};

		EventScream() {};
		EventScream(float _angleToScream, TargetTypeStepPair _info) :
			angleToScream(_angleToScream), info(_info)
		{};

		// the angle the scream was from
		float angleToScream = 0;

		// information about the TargetType of the scream and the amount of steps the scream said
		TargetTypeStepPair info;
	};
	struct EventMove final : public Event {
		std::unique_ptr<Duplicatable> duplicate() override {
			return std::unique_ptr<Duplicatable>(new EventMove(moveX, moveY));
		};

		EventMove() {};
		EventMove(float _moveX, float _moveY) :
			moveX(_moveX),
			moveY(_moveY)
		{};

		float moveX = 0.f;
		float moveY = 0.f;
	};
#pragma endregion user_defined_events
}
#pragma endregion Events
#pragma region Components
// use this file to define new component types.
// whenever you create a new type, ensure you register it in the implementation file of this header
namespace EntityComponents {
#pragma region user_defined_components_section

	struct ComponentMoveByRotation final : public Component {

		void system(Entity& entity) final;

		ComponentMoveByRotation() {
			hasSystem = true;
		};
		ComponentMoveByRotation(float _moveSpeed) :
			ComponentMoveByRotation()
		{
			moveSpeed = _moveSpeed;
		}

		float moveSpeed = 1.f;

		std::unique_ptr<Duplicatable> duplicate() override {
			return std::unique_ptr<Duplicatable>(new ComponentMoveByRotation(moveSpeed));
		};
	};
	struct ComponentBoundReflection final : public Component {

		void system(Entity& entity) final;

		ComponentBoundReflection() {
			hasSystem = true;
		};

		std::unique_ptr<Duplicatable> duplicate() override {
			return std::unique_ptr<Duplicatable>(new ComponentBoundReflection());
		};
	};
	struct ComponentPosition final : public Component {

		void system(Entity& entity) final;

		ComponentPosition() {
			hasSystem = true;
		};
		ComponentPosition(float _x, float _y) :
			ComponentPosition()
		{
			x = _x;
			y = _y;
		};
		ComponentPosition(sf::Vector2f _position) :
			ComponentPosition(_position.x, _position.y)
		{}

		float x = 0;
		float y = 0;

		std::unique_ptr<Duplicatable> duplicate() override {
			return std::unique_ptr<Duplicatable>(new ComponentPosition(x, y));
		};
	};
	struct ComponentRotation final : public Component {

		ComponentRotation() {
			hasSystem = false;
		};
		ComponentRotation(float _rotation) :
			ComponentRotation()
		{
			rotation = _rotation;
		};

		float rotation = 0.f;

		std::unique_ptr<Duplicatable> duplicate() override {
			return std::unique_ptr<Duplicatable>(new ComponentRotation(rotation));
		};
	};
	struct ComponentTargetStepTracker final : public Component {

		void system(Entity&) final;

		ComponentTargetStepTracker() {
			hasSystem = true;
		};

		TargetStepsVector targetStepsVector;

		DUPLICATE_OVERRIDE(ComponentTargetStepTracker)
	};
	struct ComponentScream final : public Component {

		void system(Entity& entity) final;

		ComponentScream() {
			hasSystem = true;
		};

		DUPLICATE_OVERRIDE(ComponentScream)
	};
	struct ComponentHearing final : public Component {

		void system(Entity& entity) final;

		ComponentHearing() {
			hasSystem = true;
		};

		DUPLICATE_OVERRIDE(ComponentHearing)
	};
	// populates the area around an entity with the given type and radius
	struct ComponentObjectGridCellPopulator final : public Component {

		void system(Entity& entity) final;

		ComponentObjectGridCellPopulator() {
			hasSystem = true;
		};
		ComponentObjectGridCellPopulator(float _popRadius, TargetType _popType) :
			ComponentObjectGridCellPopulator()
		{
			popRadius = _popRadius;
			popType = _popType;
		}

		// radius to around the entity to populate
		float popRadius = 0;
		// type to populate the cells with
		TargetType popType = TargetType::Home;

		std::unique_ptr<Duplicatable> duplicate() override {
			return std::unique_ptr<Duplicatable>(new ComponentObjectGridCellPopulator(popRadius, popType));
		};
	};
#pragma endregion user_defined_components_section
}
#pragma endregion Components

#endif