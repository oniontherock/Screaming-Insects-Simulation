#include "ECSRegistry.hpp"

uint32_t MAX_ENTITIES = 11000;
uint16_t MAX_COMPONENT_TYPES = 4;
uint16_t MAX_EVENT_TYPES = 13;

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
	EventRegistry::typeRegister<EventIDs<EventRotate>>();
	EventRegistry::typeRegister<EventIDs<EventTargetReached>>();
}

#pragma endregion Events
#pragma region Components

// registers all components,
// registering a component gives it an ID which dictates it's update order, lower ID, sooner update.
void EntityComponents::componentIDsInitialize() {

	using ComponentRegistry = TypeIDAllocator<Component>;

	ComponentRegistry::typeRegister<ComponentIDs<ComponentObjectGridCellDepopulator>>();
	ComponentRegistry::typeRegister<ComponentIDs<ComponentTargetHolder>>();
	ComponentRegistry::typeRegister<ComponentIDs<ComponentTargetStepTracker>>();
	ComponentRegistry::typeRegister<ComponentIDs<ComponentTargetCollisionChecker>>();
	ComponentRegistry::typeRegister<ComponentIDs<ComponentStepsResetOnTargetReached>>();
	ComponentRegistry::typeRegister<ComponentIDs<ComponentScream>>();
	ComponentRegistry::typeRegister<ComponentIDs<ComponentRotationRandomMovement>>();
	ComponentRegistry::typeRegister<ComponentIDs<ComponentHearing>>();
	ComponentRegistry::typeRegister<ComponentIDs<ComponentMoveByRotation>>();
	ComponentRegistry::typeRegister<ComponentIDs<ComponentChangeTargetOnTargetReached>>();
	ComponentRegistry::typeRegister<ComponentIDs<ComponentBoundReflection>>();
	ComponentRegistry::typeRegister<ComponentIDs<ComponentRotation>>();
	ComponentRegistry::typeRegister<ComponentIDs<ComponentPosition>>();
	ComponentRegistry::typeRegister<ComponentIDs<ComponentObjectGridCellPopulator>>();
}

#pragma endregion Components
#pragma region Component Templates

void EntityComponents::componentTemplatesInitialize() {
	using namespace EntityComponents;

	ComponentTemplateManager::componentTemplateAdd(

		/// template name
		"Transform",
		/// list of components in template
		{
			createComponentPairFromType<ComponentPosition>(),
			createComponentPairFromType<ComponentRotation>(),
		}
		);

	ComponentTemplateManager::componentTemplateAdd(

		/// template name
		"Insect",
		{
			"Transform"
		},
		/// list of components in template
		{
			createComponentPairFromType<ComponentBoundReflection>(),
			createComponentPairFromType<ComponentMoveByRotation>(),
			createComponentPairFromType<ComponentTargetCollisionChecker>(),
			createComponentPairFromType<ComponentTargetStepTracker>(),
			createComponentPairFromType<ComponentStepsResetOnTargetReached>(),
			createComponentPairFromType<ComponentScream>(),
			createComponentPairFromType<ComponentHearing>(),
			createComponentPairFromType<ComponentTargetHolder>(),
			createComponentPairFromType<ComponentChangeTargetOnTargetReached>(),
			createComponentPairFromType<ComponentRotationRandomMovement>(),
		}
		);
	ComponentTemplateManager::componentTemplateAdd(

		/// template name
		"Insect Scout",
		{
			"Transform"
		}, 
		/// list of components in template
		{
			createComponentPairFromType<ComponentBoundReflection>(),
			createComponentPairFromType<ComponentMoveByRotation>(),
			createComponentPairFromType<ComponentTargetCollisionChecker>(),
			createComponentPairFromType<ComponentTargetStepTracker>(),
			createComponentPairFromType<ComponentStepsResetOnTargetReached>(),
			createComponentPairFromType<ComponentScream>(),
			createComponentPairFromType<ComponentRotationRandomMovement>(),
		}
		);
	ComponentTemplateManager::componentTemplateAdd(

		/// template name
		"Target",
		{
			"Transform"
		},
		/// list of components in template
		{
			createComponentPairFromType<ComponentObjectGridCellDepopulator>(),
			createComponentPairFromType<ComponentObjectGridCellPopulator>(),
			createComponentPairFromType<ComponentRotationRandomMovement>(),
			createComponentPairFromType<ComponentBoundReflection>(),
			createComponentPairFromType<ComponentMoveByRotation>(60.f),
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
#include "GameLevel.hpp"

// if the system is not using the entity parameter, remove it's name to avoid a C4100 error

void ComponentMoveByRotation::system(Entity& entity) {
	auto* rotationComponent = entity.entityComponentGet<ComponentRotation>();

	if (rotationComponent) {
		auto* moveEvent = entity.entityEventAddAndGet<EventMove>();

		moveEvent->moveX = cos(rotationComponent->rotation) * moveSpeed * float(TimeHandler::deltaSimulatedGet());
		moveEvent->moveY = sin(rotationComponent->rotation) * moveSpeed * float(TimeHandler::deltaSimulatedGet());
	}
}
void ComponentRotation::system(Entity& entity) {
	if (entity.entityEventHas<EventRotate>()) {
		auto* rotateEvent = entity.entityEventGet<EventRotate>();

		rotation += rotateEvent->angle;
	}
}
void ComponentBoundReflection::system(Entity& entity) {

	auto* eventMove = entity.entityEventGet<EventMove>();
	if (eventMove) {

		auto* positionComponent = entity.entityComponentGet<ComponentPosition>();

		if (positionComponent) {

			float normalX = float((positionComponent->x + eventMove->moveX < 0) - (positionComponent->x + eventMove->moveX >= 1280));
			float normalY = float((positionComponent->y + eventMove->moveY < 0) - (positionComponent->y + eventMove->moveY >= 720));

			if (normalX != 0 || normalY != 0) {

				sf::Vector2f reflectedMove = Vector2fMath::reflect(eventMove->moveX, eventMove->moveY, normalX, normalY);

				eventMove->moveX = reflectedMove.x;
				eventMove->moveY = reflectedMove.y;

				auto* rotationComponent = entity.entityComponentGet<ComponentRotation>();

				auto* rotateEvent = entity.entityEventAddAndGet<EventRotate>();

				rotateEvent->angle = atan2(reflectedMove.y, reflectedMove.x) - rotationComponent->rotation;
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

	auto* positionComponent = entity.entityComponentGet<ComponentPosition>();

	if (!positionComponent) return;

	GameLevel* gameLevel = GameLevelGrid::levelGet(entity.levelId);

	auto* stepTrackerComponent = entity.entityComponentGet<ComponentTargetStepTracker>();

	if (!stepTrackerComponent) {
		std::cerr << "ERROR: Entity has ComponentScream but no ComponentTargetStepTracker" << std::endl;
		return;
	}

	for (int32_t offsetX = -1; offsetX <= 1; offsetX++) {
		for (int32_t offsetY = -1; offsetY <= 1; offsetY++) {

			if (offsetX == 0 && offsetY == 0) continue;

			sf::Vector2i gridCoordinates = gameLevel->coordinateRealToHearing(positionComponent->x + (offsetX * MAX_SCREAM_DIST), positionComponent->y + (offsetY * MAX_SCREAM_DIST));

			auto& hearingVector = gameLevel->hearingGrid[gridCoordinates.x][gridCoordinates.y];

			for (uint16_t i = 0; i < hearingVector.size(); i++) {
				Entity& entityOther = EntityManager::entitiesVector[hearingVector[i]];

				if (hearingVector[i] == entity.Id) continue;

				auto* entityOtherPositionComponent = entityOther.entityComponentGet<ComponentPosition>();

				if (!entityOtherPositionComponent) continue;

				float axisX = positionComponent->x - entityOtherPositionComponent->x;
				float axisY = positionComponent->y - entityOtherPositionComponent->y;

				if ((axisX * axisX) + (axisY * axisY) > (MAX_SCREAM_DIST * MAX_SCREAM_DIST)) continue;

				auto* screamEvent = entityOther.entityEventAddAndGet<EventScream>();

				if (screamEvent->lowestScreamsOfTypes[screamTypeCur] < (stepTrackerComponent->targetStepsVector[screamTypeCur] + MAX_SCREAM_DIST)) {
					continue;
				}

				screamEvent->lowestScreamsOfTypes[screamTypeCur] = stepTrackerComponent->targetStepsVector[screamTypeCur] + Steps(MAX_SCREAM_DIST);

				screamEvent->axesToScreams.push_back(sf::Vector2f(axisX, axisY));
				screamEvent->info.push_back(TargetTypeStepPair(screamTypeCur, Steps(stepTrackerComponent->targetStepsVector[screamTypeCur] + MAX_SCREAM_DIST)));

				gameLevel->screamConnections.push_back(ScreamConnection(
					sf::Vector2f(positionComponent->x, positionComponent->y),
					sf::Vector2f(entityOtherPositionComponent->x, entityOtherPositionComponent->y),
					screamTypeCur
				));
			}
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

		uint16_t bestInd = UINT16_MAX;

		// do we have a step tracker component? or is it nullptr?
		if (stepTrackerComponent) {
			for (uint16_t i = 0; i < screamEvent->axesToScreams.size(); i++) {

				// if it's not nullptr, is the scream event's step count less than our step count to the scream's target?
				if (screamEvent->info[i].second < stepTrackerComponent->targetStepsVector[screamEvent->info[i].first]) {
					// if yes, set our step tracker's step value of that target type to the scream's step count
					stepTrackerComponent->targetStepsVector[screamEvent->info[i].first] = screamEvent->info[i].second;

					auto* targetHolderComponent = entity.entityComponentGet<ComponentTargetHolder>();

					if (targetHolderComponent) {
						if (targetHolderComponent->targetType == screamEvent->info[i].first) {
							bestInd = i;
						}
					}
				}

				if (RNGf::probability(freedom_coefficient)) break;
			}
		}

		if (bestInd != UINT16_MAX) {
			// get our rotation component or nullptr
			auto* rotationComponent = entity.entityComponentGet<ComponentRotation>();
			// do we have a rotation component? or is it nullptr?
			if (rotationComponent) {

				auto* rotateEvent = entity.entityEventAddAndGet<EventRotate>();
				rotateEvent->angle += (atan2(screamEvent->axesToScreams[bestInd].y, screamEvent->axesToScreams[bestInd].x)) - rotationComponent->rotation;

				if (entity.entityComponentHas<ComponentPosition>()) {

					GameLevel* gameLevel = GameLevelGrid::levelGet(0, 0, 0);

					auto* positionComponent = entity.entityComponentGet<ComponentPosition>();

					sf::Vector2f position = sf::Vector2f(positionComponent->x, positionComponent->y);

					gameLevel->acceptedScreamConnections.push_back(ScreamConnection(
						position,
						position + screamEvent->axesToScreams[bestInd],
						screamEvent->info[bestInd].first
					));
				}
			}
		}


		screamEvent->axesToScreams.clear();
		screamEvent->info.clear();
		for (uint16_t i = 0; i < TargetType::TypesCount; i++) {
			screamEvent->lowestScreamsOfTypes[i] = 999999999;
		}
	}
}
void ComponentTargetStepTracker::system(Entity&) {
	for (uint8_t i = 0; i < targetStepsVector.size(); i++) {
		targetStepsVector[i] += 1;
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

				if (!ObjectGrid::gridPositionIsValidReal(positionComponent->x + offsetX, positionComponent->y + offsetY)) continue;

				Cell& cell = ObjectGrid::gridGetCellFromReal(positionComponent->x + offsetX, positionComponent->y + offsetY);

				cell.setType(popType, true);
			}
		}
	}
}
void ComponentObjectGridCellDepopulator::system(Entity& entity) {
	auto* positionComponent = entity.entityComponentGet<ComponentPosition>();

	if (positionComponent) {

		float halfRadius = popRadius / 2.f;

		for (float offsetX = -halfRadius; offsetX <= halfRadius; offsetX++) {
			for (float offsetY = -halfRadius; offsetY <= halfRadius; offsetY++) {

				if (((offsetX * offsetX) + (offsetY * offsetY)) > (halfRadius * halfRadius)) {
					continue;
				}

				if (!ObjectGrid::gridPositionIsValidReal(positionComponent->x + offsetX, positionComponent->y + offsetY)) continue;

				Cell& cell = ObjectGrid::gridGetCellFromReal(positionComponent->x + offsetX, positionComponent->y + offsetY);

				cell.clearTypes();
			}
		}
	}
}
void ComponentTargetCollisionChecker::system(Entity& entity) {

	auto* positionComponent = entity.entityComponentGet<ComponentPosition>();

	if (positionComponent) {

		if (!ObjectGrid::gridPositionIsValidReal(positionComponent->x, positionComponent->y)) {

			positionComponent->x = 640;
			positionComponent->y = 360;

			return;
		}

		Cell& cellAtPosition = ObjectGrid::gridGetCellFromReal(positionComponent->x, positionComponent->y);

		if (cellAtPosition.hasAnyType()) {
			auto* targetReachedEvent = entity.entityEventAddAndGet<EventTargetReached>();
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

			auto* rotationEvent = entity.entityEventAddAndGet<EventRotate>();
			rotationEvent->angle = Mathf::PI;
		}
	}
}
void ComponentRotationRandomMovement::system(Entity& entity) {
	auto* rotateEvent = entity.entityEventAddAndGet<EventRotate>();

	rotateEvent->angle = RNGf::getRange(Mathf::PI / 16.f);

	anglePoolCurInd = anglePoolCurInd >= ANGLE_POOL_SIZE ? 0 : anglePoolCurInd + 1;
}

#pragma endregion Systems

