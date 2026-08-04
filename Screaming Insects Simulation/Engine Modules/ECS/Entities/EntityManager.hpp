#ifndef __ENTITY_MANAGER_H__
#define __ENTITY_MANAGER_H__

#include "../../World/LevelTypeDefinitions.hpp"
#include "../Components/ComponentTemplateManager.hpp"
#include "../TypeDefinitions.hpp"
#include "Entity.hpp"
#include <vector>
#include <set>
#include <queue>


namespace EntityManager {

	extern std::vector<Entity> entitiesVector;
	extern std::set<EntityId> entityIdsSet;
	// set of entities not in a room
	extern std::set<EntityId> entitiesIntangible;
	// queue of EntityIds that have an update queued, this is used for when an entity is first created, they can queue their update even if they have a never update type
	extern std::queue<EntityId> entitiesUpdateQueued;

	void entityIdsInitialize();

	void entityUpdate(EntityId entityId);

	/**
	creates a new entity with the given id
	*/
	void entityCreateWithId(EntityId id);
	/**
	creates a new entity instance and returns it's ID

	@param updateType: the update type of the entity
	*/
	EntityId entityCreate(EntityUpdateType updateType = EntityUpdateType::Frame);
	/**
	creates a new entity instance, places it in a specified room, and returns it's ID

	@param updateType: the update type of the entity
	@param level: the LevelPosition of the level the entity should be placed in
	*/
	EntityId entityCreate(LevelPosition level, EntityUpdateType updateType = EntityUpdateType::Frame);
	/**
	creates a new entity instance, places it in a specified room, and returns it's ID

	@param updateType: the update type of the entity
	@param levelX: the X coordinate of the level the entity should be placed in
	@param levelY: the Y coordinate of the level the entity should be placed in
	@param levelZ: the Z coordinate of the level the entity should be placed in
	*/
	EntityId entityCreate(LevelCoordinate levelX, LevelCoordinate levelY, LevelCoordinate levelZ, EntityUpdateType updateType = EntityUpdateType::Frame);
	/**
	creates a new entity instance, applies a component template to it, and returns it's ID

	@param templateName: the name of the component template to be applied to the entity
	@param updateType: the update type of the entity
	*/
	EntityId entityCreate(ComponentTemplateName templateName, EntityUpdateType updateType = EntityUpdateType::Frame);
	/**
	creates a new entity instance, places it in a specified room, applies a component template to it, and returns it's ID

	@param templateName: the name of the component template to be applied to the entity
	@param updateType: the update type of the entity
	@param level: the LevelPosition of the level the entity should be placed in
	*/
	EntityId entityCreate(LevelPosition level, ComponentTemplateName templateName, EntityUpdateType updateType = EntityUpdateType::Frame);
	/**
	creates a new entity instance, places it in a specified room, applies a component template to it, and returns it's ID

	@param templateName: the name of the component template to be applied to the entity
	@param updateType: the update type of the entity
	@param levelX: the X coordinate of the level the entity should be placed in
	@param levelY: the Y coordinate of the level the entity should be placed in
	@param levelZ: the Z coordinate of the level the entity should be placed in
	*/
	EntityId entityCreate(LevelCoordinate levelX, LevelCoordinate levelY, LevelCoordinate levelZ, ComponentTemplateName templateName, EntityUpdateType updateType = EntityUpdateType::Frame);

	/**
	creates a new entity instance and returns a reference to it.

	@param updateType: the update type of the entity
	*/
	Entity& entityCreateAndGet(EntityUpdateType updateType = EntityUpdateType::Frame);
	/**
	creates a new entity instance, places it in a specified room, and returns a reference to it.

	@param updateType: the update type of the entity
	@param level: the LevelPosition of the level the entity should be placed in
	*/
	Entity& entityCreateAndGet(LevelPosition level, EntityUpdateType updateType = EntityUpdateType::Frame);
	/**
	creates a new entity instance, places it in a specified room, and returns a reference to it.

	@param updateType: the update type of the entity
	@param levelX: the X coordinate of the level the entity should be placed in
	@param levelY: the Y coordinate of the level the entity should be placed in
	@param levelZ: the Z coordinate of the level the entity should be placed in
	*/
	Entity& entityCreateAndGet(LevelCoordinate levelX, LevelCoordinate levelY, LevelCoordinate levelZ, EntityUpdateType updateType = EntityUpdateType::Frame);
	/**
	creates a new entity instance, applies a component template to it, and returns a reference to it.

	@param templateName: the name of the component template to be applied to the entity
	@param updateType: the update type of the entity
	*/
	Entity& entityCreateAndGet(ComponentTemplateName templateName, EntityUpdateType updateType = EntityUpdateType::Frame);
	/**
	creates a new entity instance, places it in a specified room, applies a component template to it, and returns a reference to it.

	@param templateName: the name of the component template to be applied to the entity
	@param updateType: the update type of the entity
	@param level: the LevelPosition of the level the entity should be placed in
	*/
	Entity& entityCreateAndGet(LevelPosition level, ComponentTemplateName templateName, EntityUpdateType updateType = EntityUpdateType::Frame);
	/**
	creates a new entity instance, places it in a specified room, applies a component template to it, and returns a reference to it.

	@param templateName: the name of the component template to be applied to the entity
	@param updateType: the update type of the entity
	@param levelX: the X coordinate of the level the entity should be placed in
	@param levelY: the Y coordinate of the level the entity should be placed in
	@param levelZ: the Z coordinate of the level the entity should be placed in
	*/
	Entity& entityCreateAndGet(LevelCoordinate levelX, LevelCoordinate levelY, LevelCoordinate levelZ, ComponentTemplateName templateName, EntityUpdateType updateType = EntityUpdateType::Frame);

	void entityAddToRoom(EntityId entityId, LevelPosition level);
	void entityAddToRoom(EntityId entityId, LevelCoordinate levelX, LevelCoordinate levelY, LevelCoordinate levelZ);

	void entityRemoveFromRoom(EntityId entityId, LevelPosition level);
	void entityRemoveFromRoom(EntityId entityId, LevelCoordinate levelX, LevelCoordinate levelY, LevelCoordinate levelZ);


	Entity& entityGet(EntityId entityId);
	void entityTerminate(EntityId entityID);
	void entitiesAllDelete();

	void entitiesIntangibleUpdate();
	// update entities with an update queued
	void entitiesQueuedUpdate();
};

#endif