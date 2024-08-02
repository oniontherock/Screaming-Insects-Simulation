#include "ECSRegistry.hpp"

uint32_t MAX_ENTITIES = 11000;
uint16_t MAX_COMPONENT_TYPES = 14;
uint16_t MAX_EVENT_TYPES = 4;

void ECSRegistry::ECSInitialize() {
	EntityComponents::componentIDsInitialize();
	EntityComponents::componentTemplatesInitialize();
	EntityEvents::eventIDsInitialize();
	EventPool::eventPoolInitialize();
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

	ComponentRegistry::typeRegister<ComponentIDs<ComponentVariableRandomizer>>();
	ComponentRegistry::typeRegister<ComponentIDs<ComponentObjectGridCellDepopulator>>();
	ComponentRegistry::typeRegister<ComponentIDs<ComponentTargetHolder>>();
	ComponentRegistry::typeRegister<ComponentIDs<ComponentTargetCollisionChecker>>();
	ComponentRegistry::typeRegister<ComponentIDs<ComponentStepsResetOnTargetReached>>();
	ComponentRegistry::typeRegister<ComponentIDs<ComponentRotationRandomMovement>>();
	ComponentRegistry::typeRegister<ComponentIDs<ComponentChangeTargetOnTargetReached>>();
	ComponentRegistry::typeRegister<ComponentIDs<ComponentMoveByRotation>>();
	ComponentRegistry::typeRegister<ComponentIDs<ComponentBoundReflection>>();
	ComponentRegistry::typeRegister<ComponentIDs<ComponentPosition>>();
	ComponentRegistry::typeRegister<ComponentIDs<ComponentTargetStepTracker>>();
	ComponentRegistry::typeRegister<ComponentIDs<ComponentScream>>();
	ComponentRegistry::typeRegister<ComponentIDs<ComponentHearing>>();
	ComponentRegistry::typeRegister<ComponentIDs<ComponentRotation>>();
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
		"MoveAndReflect",
		/// list of components in template
		{
			createComponentPairFromType<ComponentMoveByRotation>(),
			createComponentPairFromType<ComponentBoundReflection>(),
		}
		);

	ComponentTemplateManager::componentTemplateAdd(

		/// template name
		"Insect",
		{
			"Transform",
			"MoveAndReflect",
		},
		/// list of components in template
		{
			createComponentPairFromType<ComponentVariableRandomizer>([](Entity& entity) {
				auto* entityPositionComponent = entity.entityComponentGet<ComponentPosition>();
				entityPositionComponent->x = RNGf::getRange(16, 1280 - 16);
				entityPositionComponent->y = RNGf::getRange(16, 720 - 16);
				entity.entityComponentGet<ComponentRotation>()->rotation = RNGf::getFullRange(Mathf::PI);
				entity.entityComponentGet<ComponentMoveByRotation>()->moveSpeed = RNGf::getRange(16.f) + 80.f;
				entity.entityComponentTerminate<ComponentVariableRandomizer>();
				}),
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
			"Transform",
			"MoveAndReflect",
		}, 
		/// list of components in template
		{
			createComponentPairFromType<ComponentVariableRandomizer>([](Entity& entity) {
				auto* entityPositionComponent = entity.entityComponentGet<ComponentPosition>();
				entityPositionComponent->x = RNGf::getRange(16, 1280 - 16);
				entityPositionComponent->y = RNGf::getRange(16, 720 - 16);
				entity.entityComponentGet<ComponentRotation>()->rotation = RNGf::getFullRange(Mathf::PI);
				entity.entityComponentGet<ComponentMoveByRotation>()->moveSpeed = RNGf::getRange(16.f) + 80.f;
				entity.entityComponentTerminate<ComponentVariableRandomizer>();
				}),
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
			//createComponentPairFromType<ComponentMoveByRotation>(30.f),
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

void ComponentVariableRandomizer::system(Entity& entity) {
	std::invoke(randomizeFunction, entity);
}
void ComponentMoveByRotation::system(Entity& entity) {
	auto* rotationComponent = entity.entityComponentGet<ComponentRotation>();

	if (rotationComponent) {
		
		float rotation = rotationComponent->rotation;
		
		auto* moveEvent = entity.entityEventAddAndGet<EventMove>();

		if (entity.entityEventHas<EventRotate>()) {
			auto rotateEvents = entity.entityEventGetAllOfType<EventRotate>();

			for (uint16_t i = 0; i < rotateEvents.size(); i++) {
				rotation += rotateEvents[i]->angle;
			}
		}

		moveEvent->moveX = cos(rotation) * moveSpeed * float(TimeHandler::deltaSimulatedGet());
		moveEvent->moveY = sin(rotation) * moveSpeed * float(TimeHandler::deltaSimulatedGet());
	}
}
void ComponentRotation::system(Entity& entity) {
	if (entity.entityEventHas<EventRotate>()) {
		auto rotateEvents = entity.entityEventGetAllOfType<EventRotate>();

		for (uint16_t i = 0; i < rotateEvents.size(); i++) {
			rotation += rotateEvents[i]->angle;
		}

	}
}
void ComponentBoundReflection::system(Entity& entity) {

	if (entity.entityEventHas<EventMove>()) {
		auto moveEvents = entity.entityEventGetAllOfType<EventMove>();

		sf::Vector2f finalMoveAmount{};

		for (uint16_t i = 0; i < moveEvents.size(); i++) {
			finalMoveAmount.x += moveEvents[i]->moveX;
			finalMoveAmount.y += moveEvents[i]->moveY;
		}
		entity.entityEventTerminateAllOfType<EventMove>();

		auto* positionComponent = entity.entityComponentGet<ComponentPosition>();

		if (positionComponent) {

			float normalX = float((positionComponent->x + finalMoveAmount.x < 0) - (positionComponent->x + finalMoveAmount.x >= 1280));
			float normalY = float((positionComponent->y + finalMoveAmount.y < 0) - (positionComponent->y + finalMoveAmount.y >= 720));

			if (normalX != 0 || normalY != 0) {

				sf::Vector2f reflectedMove = Vector2fMath::reflect(finalMoveAmount.x, finalMoveAmount.y, normalX, normalY);

				finalMoveAmount.x = reflectedMove.x;
				finalMoveAmount.y = reflectedMove.y;

				auto* rotationComponent = entity.entityComponentGet<ComponentRotation>();

				auto* rotateEvent = entity.entityEventAddAndGet<EventRotate>();

				rotateEvent->angle = atan2(reflectedMove.y, reflectedMove.x) - rotationComponent->rotation;
			}
		}

		auto* moveEvent = entity.entityEventAddAndGet<EventMove>();
	
		moveEvent->moveX = finalMoveAmount.x;
		moveEvent->moveY = finalMoveAmount.y;
	}
}
void ComponentPosition::system(Entity& entity) {
	if (entity.entityEventHas<EventMove>()) {

		auto moveEvents = entity.entityEventGetAllOfType<EventMove>();

		for (uint16_t i = 0; i < moveEvents.size(); i++) {
			x += moveEvents[i]->moveX;
			y += moveEvents[i]->moveY;
		}
	}
}
void ComponentScream::system(Entity& entity) {

	auto* positionComponent = entity.entityComponentGet<ComponentPosition>();

	if (!positionComponent) return;

	GameLevel* gameLevel = GameLevelGrid::levelGet(entity.levelId);

	auto* stepTrackerComponent = entity.entityComponentGet<ComponentTargetStepTracker>();

	if (!stepTrackerComponent) {
		ConsoleHandler::consolePrintErr("Entity has ComponentScream but no ComponentTargetStepTracker");
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

				screamEvent->axisToScream = sf::Vector2f(axisX, axisY);
				screamEvent->info = TargetTypeStepPair(screamTypeCur, Steps(stepTrackerComponent->targetStepsVector[screamTypeCur] + MAX_SCREAM_DIST));

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
		auto screamEvents = entity.entityEventGetAllOfType<EventScream>();

		// get the entity's target step tracker component or nullptr
		auto* stepTrackerComponent = entity.entityComponentGet<ComponentTargetStepTracker>();

		uint16_t bestInd = UINT16_MAX;

		// do we have a step tracker component? or is it nullptr?
		if (stepTrackerComponent) {
			for (uint16_t i = 0; i < screamEvents.size(); i++) {

				auto* screamEventCur = static_cast<EventScream*>(screamEvents[i]);

				Steps curSteps = screamEventCur->info.second;

				// if it's not nullptr, is the scream event's step count less than our step count to the scream's target?
				if (curSteps < stepTrackerComponent->targetStepsVector[screamEventCur->info.first]) {
					// if yes, set our step tracker's step value of that target type to the scream's step count
					stepTrackerComponent->targetStepsVector[screamEventCur->info.first] = curSteps;

					auto* targetHolderComponent = entity.entityComponentGet<ComponentTargetHolder>();

					if (targetHolderComponent) {
						if (targetHolderComponent->targetType == screamEventCur->info.first) {
							bestInd = i;
						}
					}
				}

				if (RNGf::probability(freedom_coefficient)) break;
			}
		}

		if (bestInd != UINT16_MAX) {
		
			auto* screamEventBest = static_cast<EventScream*>(screamEvents[bestInd]);
			
			// get our rotation component or nullptr
			auto* rotationComponent = entity.entityComponentGet<ComponentRotation>();
			// do we have a rotation component? or is it nullptr?
			if (rotationComponent) {

				auto* rotateEvent = entity.entityEventAddAndGet<EventRotate>();
				rotateEvent->angle = atan2(screamEventBest->axisToScream.y, screamEventBest->axisToScream.x) - rotationComponent->rotation;

				if (entity.entityComponentHas<ComponentPosition>()) {

					GameLevel* gameLevel = GameLevelGrid::levelGet(0, 0, 0);

					auto* positionComponent = entity.entityComponentGet<ComponentPosition>();

					sf::Vector2f position = sf::Vector2f(positionComponent->x, positionComponent->y);

					gameLevel->acceptedScreamConnections.push_back(ScreamConnection(
						position,
						position + screamEventBest->axisToScream,
						screamEventBest->info.first
					));
				}
			}
		}
	}
}
void ComponentTargetStepTracker::system(Entity& entity) {
	if (entity.entityEventHas<EventMove>()) {

		auto moveEvents = entity.entityEventGetAllOfType<EventMove>();

		float finalMoveDist = 0;

		for (uint16_t i = 0; i < moveEvents.size(); i++) {
			finalMoveDist += (moveEvents[i]->moveX * moveEvents[i]->moveX) + (moveEvents[i]->moveY * moveEvents[i]->moveY);
		}

		finalMoveDist = ceil(sqrt(finalMoveDist));

		for (uint8_t i = 0; i < targetStepsVector.size(); i++) {
			targetStepsVector[i] += Steps(finalMoveDist);
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


	if (entity.entityEventHas<EventTargetReached>()) {
		
		auto* targetReachedEvent = entity.entityEventGet<EventTargetReached>();
		
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

			entity.entityEventTerminateAllOfType<EventRotate>();

			auto* rotateEvent = entity.entityEventAddAndGet<EventRotate>();
			rotateEvent->angle = Mathf::PI;
		}
	}
}
void ComponentRotationRandomMovement::system(Entity& entity) {
	auto* rotateEvent = entity.entityEventAddAndGet<EventRotate>();

	rotateEvent->angle = RNGf::getRange(Mathf::PI / 16.f);

	anglePoolCurInd = anglePoolCurInd >= ANGLE_POOL_SIZE ? 0 : anglePoolCurInd + 1;
}

#pragma endregion Systems

