#include "ECSRegistry.hpp"

void ECSRegistry::ECSInitialize() {
	EntityComponents::componentIDsInitialize();
	EntityComponents::componentTemplatesInitialize();
	EntityEvents::eventIDsInitialize();
}
void ECSRegistry::ECSTerminate() {
	EntityEvents::allEventsTerminate();
	EntityManager::entitiesAllDelete();
}

#pragma region Events
/*
registers all events
registering an event gives it an ID which dictates it's update order, lower ID, sooner update.

the order of registry is very important, as it heavily dictates the behavior of events,
for example:

REGISTER(Event, EventA)
REGISTER(Event, EventB)

in this example, EventA is ALWAYS updated BEFORE EventB,
which is very important, because if EventA sends and event, EventB will always receive it,
but if the order were swapped, EventB would never receive it
*/
void EntityEvents::eventIDsInitialize() {

	using EventRegistry = TypeIDAllocator<Event>;

	/// registry convention:
	EventRegistry::typeRegister<EventIDs<EventScream>>();
	EventRegistry::typeRegister<EventIDs<EventMove>>();
}

#pragma endregion Events
#pragma region Components
/*
registers all components,
registering a component gives it an ID which dictates it's update order, lower ID, sooner update.

the order of registry is very important, as it heavily dictates the behavior of components,
for example:

REGISTER(Component, ComponentA)
REGISTER(Component, ComponentB)

in this example, ComponentA is ALWAYS updated BEFORE ComponentB,
which is very important, because if ComponentA sends and event, ComponentB will always receive it,
but if the order were swapped, ComponentB would never receive it
*/


void EntityComponents::componentIDsInitialize() {

	using ComponentRegistry = TypeIDAllocator<Component>;

	/// registry convention:
	ComponentRegistry::typeRegister<ComponentIDs<ComponentRotation>>();
	ComponentRegistry::typeRegister<ComponentIDs<ComponentMoveByRotation>>();
	ComponentRegistry::typeRegister<ComponentIDs<ComponentBoundReflection>>();
	ComponentRegistry::typeRegister<ComponentIDs<ComponentPosition>>();
	ComponentRegistry::typeRegister<ComponentIDs<ComponentTargetStepTracker>>();
	ComponentRegistry::typeRegister<ComponentIDs<ComponentScream>>();
	ComponentRegistry::typeRegister<ComponentIDs<ComponentHearing>>();
}

#pragma endregion Components
#pragma region Component Templates

void EntityComponents::componentTemplatesInitialize() {
	using namespace EntityComponents;

	ComponentTemplateManager::componentTemplateAdd(

		/// template name
		"Insect",
		/// list of components in template
		{
			createComponentPairFromType<ComponentPosition>(),
			createComponentPairFromType<ComponentBoundReflection>(),
			createComponentPairFromType<ComponentMoveByRotation>(),
			createComponentPairFromType<ComponentRotation>(),
			createComponentPairFromType<ComponentTargetStepTracker>(),
			createComponentPairFromType<ComponentScream>(),
			createComponentPairFromType<ComponentHearing>(),
		}
		);
}

#pragma endregion Component Templates
#pragma region Systems

using namespace EntityComponents;
using namespace EntityEvents;

// if you need to include a certain file for a system, include it here.
#include <iostream>
#include "../Include/Common/Math.hpp"

// if the system is not using the entity parameter, remove it's name to avoid a C4100 error

void ComponentMoveByRotation::system(Entity& entity) {
	auto* rotationComponent = entity.entityComponentGet<ComponentRotation>();

	if (rotationComponent) {
		auto* moveEvent = entity.entityEventAddAndReturn<EventMove>();

		moveEvent->moveX = cos(rotationComponent->rotation) * moveSpeed;
		moveEvent->moveY = sin(rotationComponent->rotation) * moveSpeed;
	}
}
void ComponentBoundReflection::system(Entity& entity) {

	auto* eventMove = entity.entityEventGet<EventMove>();
	if (eventMove) {

		auto* positionComponent = entity.entityComponentGet<ComponentPosition>();
		auto* rotationComponent = entity.entityComponentGet<ComponentRotation>();

		if (positionComponent) {

			float normalX = float((positionComponent->x + eventMove->moveX < 0) - (positionComponent->x + eventMove->moveX >= 1280));
			float normalY = float((positionComponent->y + eventMove->moveY < 0) - (positionComponent->y + eventMove->moveY >= 720));

			if (normalX != 0 || normalY != 0) {

				sf::Vector2f reflectedMove = Vector2fMath::reflect(eventMove->moveX, eventMove->moveY, normalX, normalY);

				eventMove->moveX = reflectedMove.x;
				eventMove->moveY = reflectedMove.y;

				rotationComponent->rotation = atan2(reflectedMove.y, reflectedMove.x);
			}
		}
	}
}
void ComponentPosition::system(Entity& entity) {
	if (entity.entityEventHas<EventMove>()) {
		auto* moveEvent = entity.entityEventGet<EventMove>();

		x += moveEvent->moveX;
		y += moveEvent->moveY;
	}
}
void ComponentScream::system(Entity& entity) {
}
void ComponentHearing::system(Entity& entity) {

	// has there been a scream?
	if (entity.entityEventHas<EventScream>()) {
		// if yes, get the scream event
		auto* screamEvent = entity.entityEventGet<EventScream>();

		// get the entity's target step tracker component or nullptr
		auto* stepTrackerComponent = entity.entityComponentGet<ComponentTargetStepTracker>();

		// do we have a step tracker component? or is it nullptr?
		if (stepTrackerComponent) {
			// if it's not nullptr, is the scream event's step count less than our step count to the scream's target?
			if (screamEvent->info.second < stepTrackerComponent->targetStepsVector[screamEvent->info.first]) {
				// if yes, set our step tracker's step value of that target type to the scream's step count
				stepTrackerComponent->targetStepsVector[screamEvent->info.first] = screamEvent->info.second;

				// get our rotation component or nullptr
				auto* rotationComponent = entity.entityComponentGet<ComponentRotation>();
				// do we have a rotation component? or is it nullptr?
				if (rotationComponent) {
					// if we do have a rotation component, set our rotation to the angle to the scream
					rotationComponent->rotation = screamEvent->angleToScream;
				}
			}
		}
	}
}
void ComponentTargetStepTracker::system(Entity& entity) {
	if (entity.entityEventHas<EventMove>()) {
		
		auto* moveEvent = entity.entityEventGet<EventMove>();

		uint16_t moveSpeed = uint16_t(std::ceil(std::sqrt((moveEvent->moveX * moveEvent->moveX) + (moveEvent->moveY * moveEvent->moveY))));

		for (uint8_t i = 0; i < targetStepsVector.size(); i++) {
			targetStepsVector[i] += moveSpeed;
		}
	}
}

#pragma endregion Systems

