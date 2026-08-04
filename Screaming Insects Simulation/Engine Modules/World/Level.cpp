#include "Level.hpp"
#include "../ECS/Entities/EntityManager.hpp"

BaseLevel::BaseLevel() :
	entities(std::vector<EntityId>()),
	isUpdating(true)
{}

BaseLevel::BaseLevel(LevelCoordinate _idX, LevelCoordinate _idY, LevelCoordinate _idZ) :
	BaseLevel()
{
	levelPosition = LevelPosition(_idX, _idY, _idZ);
}

BaseLevel::BaseLevel(LevelPosition _id) :
	BaseLevel(_id.x, _id.y, _id.z)
{}

void BaseLevel::entityIdAdd(EntityId id) {
	if (EntityManager::entityGet(id).updateType == EntityUpdateType::Frame) entities.push_back(id);
	else if (EntityManager::entityGet(id).updateType == EntityUpdateType::Never) entitiesNoUpdate.push_back(id);
	else if (EntityManager::entityGet(id).updateType == EntityUpdateType::Observation) entitiesObservation.push_back(id);
}
void BaseLevel::entityIdRemove(const EntityId id) {

	if (EntityManager::entityGet(id).updateType == EntityUpdateType::Frame) {
		// the position of the id in the entities vector
		auto idVectorPosition = std::find(entities.begin(), entities.end(), id);

		if (idVectorPosition != entities.end()) {
			entities.erase(idVectorPosition);
		}
	}
	else if (EntityManager::entityGet(id).updateType == EntityUpdateType::Never) {
		// the position of the id in the entitiesNoUpdate vector
		auto idVectorPosition = std::find(entitiesNoUpdate.begin(), entitiesNoUpdate.end(), id);

		if (idVectorPosition == entitiesNoUpdate.end()) {
			entitiesNoUpdate.erase(idVectorPosition);
		}
	}
	else if (EntityManager::entityGet(id).updateType == EntityUpdateType::Observation) {
		// the position of the id in the entitiesObservation vector
		auto idVectorPosition = std::find(entitiesObservation.begin(), entitiesObservation.end(), id);

		if (idVectorPosition == entitiesObservation.end()) {
			entitiesObservation.erase(idVectorPosition);
		}
	}
}

void BaseLevel::entitiesObservedUpdate() {};
