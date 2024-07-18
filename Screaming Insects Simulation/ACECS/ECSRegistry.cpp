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

// registers all events
// registering an event gives it an ID which dictates it's update order, lower ID, sooner update.
void EntityEvents::eventIDsInitialize() {

	using EventRegistry = TypeIDAllocator<Event>;

	EventRegistry::typeRegister<EventIDs<EventScream>>();
	EventRegistry::typeRegister<EventIDs<EventMove>>();
	EventRegistry::typeRegister<EventIDs<EventTargetReached>>();
}

#pragma endregion Events
#pragma region Components

// registers all components,
// registering a component gives it an ID which dictates it's update order, lower ID, sooner update.
void EntityComponents::componentIDsInitialize() {

	using ComponentRegistry = TypeIDAllocator<Component>;

	ComponentRegistry::typeRegister<ComponentIDs<ComponentTargetHolder>>();
	ComponentRegistry::typeRegister<ComponentIDs<ComponentRotation>>();
	ComponentRegistry::typeRegister<ComponentIDs<ComponentMoveByRotation>>();
	ComponentRegistry::typeRegister<ComponentIDs<ComponentBoundReflection>>();
	ComponentRegistry::typeRegister<ComponentIDs<ComponentPosition>>();
	ComponentRegistry::typeRegister<ComponentIDs<ComponentTargetStepTracker>>();
	ComponentRegistry::typeRegister<ComponentIDs<ComponentTargetCollisionChecker>>();
	ComponentRegistry::typeRegister<ComponentIDs<ComponentStepsResetOnTargetReached>>();
	ComponentRegistry::typeRegister<ComponentIDs<ComponentChangeTargetOnTargetReached>>();
	ComponentRegistry::typeRegister<ComponentIDs<ComponentScream>>();
	ComponentRegistry::typeRegister<ComponentIDs<ComponentHearing>>();
	ComponentRegistry::typeRegister<ComponentIDs<ComponentObjectGridCellPopulator>>();
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
			createComponentPairFromType<ComponentTargetCollisionChecker>(),
			createComponentPairFromType<ComponentTargetStepTracker>(),
			createComponentPairFromType<ComponentStepsResetOnTargetReached>(),
			createComponentPairFromType<ComponentScream>(),
			createComponentPairFromType<ComponentHearing>(),
			createComponentPairFromType<ComponentTargetHolder>(),
			createComponentPairFromType<ComponentChangeTargetOnTargetReached>(),
		}
		);
	ComponentTemplateManager::componentTemplateAdd(

		/// template name
		"Target",
		/// list of components in template
		{
			createComponentPairFromType<ComponentPosition>(),
			createComponentPairFromType<ComponentObjectGridCellPopulator>(),
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
#include "../Include/Common/TimeHandler.hpp"
#include "../Include/Simulation/Object Grid/ObjectGrid.hpp"

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

		x += moveEvent->moveX * TimeHandler::deltaRealGet();
		y += moveEvent->moveY * TimeHandler::deltaRealGet();
	}
}
void ComponentScream::system(Entity& entity) {

	auto* positionComponent = entity.entityComponentGet<ComponentPosition>();

	if (!positionComponent) return;

	BaseLevel* entityLevel = WorldGrid::levelGet(entity.levelId);

	for (uint16_t i = 0; i < entityLevel->entities.size(); i++) {
		Entity& entityOther = EntityManager::entitiesVector[entityLevel->entities[i]];

		if (entityLevel->entities[i] == entity.Id) continue;

		auto* entityOtherPositionComponent = entityOther.entityComponentGet<ComponentPosition>();

		if (!entityOtherPositionComponent) continue;

		float axisX = positionComponent->x - entityOtherPositionComponent->x;
		float axisY = positionComponent->y - entityOtherPositionComponent->y;

		float axisLenSqrd = (axisX * axisX) + (axisY * axisY);

		if (axisLenSqrd > (MAX_SCREAM_DIST * MAX_SCREAM_DIST)) continue;


		auto* screamEvent = entityOther.entityEventAddAndReturn<EventScream>();

		auto* stepTrackerComponent = entity.entityComponentGet<ComponentTargetStepTracker>();

		if (stepTrackerComponent) {
			screamEvent->anglesToScreams.push_back(atan2(axisY, axisX));
			screamEvent->info.push_back(TargetTypeStepPair(screamTypeCur, stepTrackerComponent->targetStepsVector[screamTypeCur] + MAX_SCREAM_DIST));
		}
		else {
			std::cerr << "ERROR: Entity has ComponentScream but no ComponentTargetStepTracker, scream sent with error values" << std::endl;
			screamEvent->anglesToScreams.push_back(0);
			screamEvent->info.push_back(TargetTypeStepPair(TargetType::TypesCount, 999999999999));
		}
	}

	screamTypeCur = static_cast<TargetType>((screamTypeCur + 1) < TargetType::TypesCount ? screamTypeCur + 1 : 0);

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
			for (uint16_t i = 0; i < screamEvent->anglesToScreams.size(); i++) {
				// if it's not nullptr, is the scream event's step count less than our step count to the scream's target?
				if (screamEvent->info[i].second < stepTrackerComponent->targetStepsVector[screamEvent->info[i].first]) {
					// if yes, set our step tracker's step value of that target type to the scream's step count
					stepTrackerComponent->targetStepsVector[screamEvent->info[i].first] = screamEvent->info[i].second;

					auto* targetHolderComponent = entity.entityComponentGet<ComponentTargetHolder>();

					if (targetHolderComponent) {
						if (targetHolderComponent->targetType == screamEvent->info[i].first) {
							// get our rotation component or nullptr
							auto* rotationComponent = entity.entityComponentGet<ComponentRotation>();
							// do we have a rotation component? or is it nullptr?
							if (rotationComponent) {
								// if we do have a rotation component, set our rotation to the angle to the scream
								rotationComponent->rotation = screamEvent->anglesToScreams[i];
							}
						}
					}
				}
			}

			screamEvent->anglesToScreams.clear();
			screamEvent->info.clear();
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
void ComponentObjectGridCellPopulator::system(Entity& entity) {
	
	auto* positionComponent = entity.entityComponentGet<ComponentPosition>();

	if (positionComponent) {

		float halfRadius = popRadius / 2.f;

		for (float offsetX = -halfRadius; offsetX <= halfRadius; offsetX++) {
			for (float offsetY = -halfRadius; offsetY <= halfRadius; offsetY++) {

				if (((offsetX * offsetX) + (offsetY * offsetY)) > (halfRadius * halfRadius)) {
					continue;
				}

				Cell& cell = ObjectGrid::gridGetCellFromReal(positionComponent->x + offsetX, positionComponent->y + offsetY);

				cell.setType(popType, true);
			}
		}
	}
}
void ComponentTargetCollisionChecker::system(Entity& entity) {

	auto* positionComponent = entity.entityComponentGet<ComponentPosition>();

	if (positionComponent) {

		Cell& cellAtPosition = ObjectGrid::gridGetCellFromReal(positionComponent->x, positionComponent->y);

		if (cellAtPosition.hasAnyType()) {
			auto* targetReachedEvent = entity.entityEventAddAndReturn<EventTargetReached>();
			targetReachedEvent->type = cellAtPosition.getFirstType();
		}
	}
}
void ComponentStepsResetOnTargetReached::system(Entity& entity) {
	if (entity.entityEventHas<EventTargetReached>()) {
		auto* stepTrackerComponent = entity.entityComponentGet<ComponentTargetStepTracker>();

		if (stepTrackerComponent) {
			stepTrackerComponent->targetStepsVector[entity.entityEventGet<EventTargetReached>()->type] = 0;
		}
	}
}
void ComponentChangeTargetOnTargetReached::system(Entity& entity) {

	auto* targetReachedEvent = entity.entityEventGet<EventTargetReached>();

	if (targetReachedEvent) {
		auto* targetHolderComponent = entity.entityComponentGet<ComponentTargetHolder>();

		if (targetHolderComponent) {
			if (targetHolderComponent->targetType == TargetType::Home && targetReachedEvent->type == Home) {
				targetHolderComponent->targetType = TargetType::Food;
			}
			else if (targetHolderComponent->targetType != TargetType::Home && targetReachedEvent->type != Home) {
				targetHolderComponent->targetType = TargetType::Home;
			}
			else {
				return;
			}

			if (entity.entityComponentHas<ComponentRotation>()) {
				entity.entityComponentGet<ComponentRotation>()->rotation += Mathf::PI;
			}
		}
	}
}

#pragma endregion Systems

