#ifndef __LEVEL_H__
#define __LEVEL_H__

#include "../ECS/TypeDefinitions.hpp"
#include "LevelTypeDefinitions.hpp"
#include <iostream>
#include <vector>


struct BaseLevel {

	BaseLevel();
	BaseLevel(LevelCoordinate _idX, LevelCoordinate _idY, LevelCoordinate _idZ);
	BaseLevel(LevelPosition _id);

	LevelPosition levelPosition;

	void entityIdAdd(EntityId id);
	void entityIdRemove(const EntityId id);

	bool isUpdating;

	// vector of EntityIDs belonging to this room whose update type is Frame
	std::vector<EntityId> entities;
	// vector of EntityIDs belonging to this room whose update type is Never
	std::vector<EntityId> entitiesNoUpdate;
	// vector of EntityIDs belonging to this room whose update type is Observed
	std::vector<EntityId> entitiesObservation;
	// vector of EntityIDs belonging to this room who are currently being observed,
	// the way this is updated is dependent on the project, so we have the entitiesObservedUpdate function for this purpose.
	std::vector<EntityId> entitiesObserved;

	// updates the entitiesObserved vector,
	// this function's implementation in BaseLevel does nothing since it is meant to be defined by GameLevel dependant on the specific game.
	virtual void entitiesObservedUpdate();
};

template <class Level>
using LevelPtr = std::unique_ptr<Level>;

using BaseLevelPtr = LevelPtr<BaseLevel>;

#endif
