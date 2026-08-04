#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "../../World/LevelTypeDefinitions.hpp"
#include "../Components/Component.hpp"
#include "../Components/ComponentIDs.hpp"
#include "../Events/Event.hpp"
#include "../Events/EventIDs.hpp"
#include "../Events/EventPool.hpp"
#include "../TypeDefinitions.hpp"
#include <cstdint>
#include <vector>



typedef std::vector<EventUniquePtr> EventVector;

// used for vectors of events
template <class CastType>
class CastedVector {

	EventVector& vecToCast;

public:
	CastedVector(EventVector& _vecToCast) :
		vecToCast(_vecToCast)
	{}

	inline CastType* operator[] (int ind) {
		return dynamic_cast<CastType*>(vecToCast[ind].get());
	}

	inline size_t size() const { return vecToCast.size(); }

};

struct Entity {
private:
	bool hasAnyEvent = false;

	void componentsAllTerminate();
	void eventsAllTerminate();
public:

	void terminate();

	explicit Entity();
	explicit Entity(EntityId _id, EntityUpdateType _updateType);
	explicit Entity(EntityId _id, EntityUpdateType _updateType, LevelPosition _levelId);

	Entity(Entity& other);
	Entity& operator= (const Entity& other);
	~Entity();

	void componentsInitialize();
	void eventsInitialize();

	EntityUpdateType updateType;

	// the Id of this entity, every entity has a unique Id.
	// Ids are used to access entities in the EntityManager's "entities" vector
	EntityId Id;
	// the id of the level that this entity belongs to,
	// if all three components are equal to the max uint32_t value, then this entity does not belong to a level
	LevelPosition levelId = NoLevelPosition;

	std::vector<ComponentUniquePtr> componentsVector;

	std::vector<EventVector> eventsVector;


	template <class T>
	T* entityComponentGet() {
		return static_cast<T*>(componentsVector[EntityComponents::ComponentIDs<T>::ID].get());
	}
	template <class T>
	bool entityComponentHas() {
		return static_cast<bool>(componentsVector[EntityComponents::ComponentIDs<T>::ID]);
	}
	template <class T>
	void entityComponentAdd(EntityComponents::Component* component) {
		componentsVector[EntityComponents::ComponentIDs<T>::ID] = ComponentUniquePtr(component);
	}
	// checks if the entity has the component, and if not, adds it
	template <class T>
	void entityComponentAddNoOverwrite(EntityComponents::Component* component) {
		if (entityComponentHas<T>()) return;

		entityComponentAdd<T>(component);
	}
	// resets the unique_ptr for the specified component type, thus terminating the component
	template <class T>
	void entityComponentTerminate() {
		componentsVector[EntityComponents::ComponentIDs<T>::ID].reset();
	}
	bool entityComponentHasAtIndex(EntityComponents::ComponentTypeID index);
	EntityComponents::Component* entityComponentGetAtIndex(EntityComponents::ComponentTypeID index);
	void entityComponentAddAtIndex(EntityComponents::Component* component, EntityComponents::ComponentTypeID index);
	// checks if the entity has the component, and if not, adds it
	void entityComponentAddAtIndexNoOverwrite(EntityComponents::Component* component, EntityComponents::ComponentTypeID index);


	template <class T>
	T* entityEventGet(uint16_t ind) {
		return static_cast<T*>(eventsVector[EntityEvents::EventIDs<T>::ID][ind].get());
	}
	template <class T>
	T* entityEventGet() {
		return entityEventGet<T>(0);
	}
	template <class T>
	T* entityEventGetSafe(uint16_t ind) {
		if (eventsVector[EntityEvents::EventIDs<T>::ID].size() <= 0) return nullptr;

		return entityEventGet(ind);
	}
	template <class T>
	T* entityEventGetSafe() {
		return entityEventGetSafe<T>(0);
	}
	template <class T>
	bool entityEventHas() {
		return eventsVector[EntityEvents::EventIDs<T>::ID].size() > 0;
	}
	// sets an event to active
	template <class T>
	void entityEventAdd() {
		hasAnyEvent = true;
		eventsVector[EntityEvents::EventIDs<T>::ID].push_back(EventPool::eventPoolTake<T>());
	}
	// sets an event to active and returns the event so it's members may be modified
	template <class T>
	T* entityEventAddAndGet() {
		EntityEvents::EventTypeID eventId = EntityEvents::EventIDs<T>::ID;
		if (eventId >= eventsVector.size()) eventsVector.resize(eventId + 1);
		entityEventAdd<T>();
		return static_cast<T*>(eventsVector[eventId].back().get());
	}
	void entityEventTerminate(EntityEvents::EventTypeID eventId, uint16_t ind);

	template <class T>
	void entityEventTerminate(uint16_t ind) {
		entityEventTerminate(EntityEvents::EventIDs<T>::ID, ind);
	}
	template <class T>
	CastedVector<T> entityEventGetAllOfType() {
		EntityEvents::EventTypeID eventId = EntityEvents::EventIDs<T>::ID;
		if (eventId >= eventsVector.size()) {
			eventsVector.resize(eventId + 1);
		}
		return CastedVector<T>(eventsVector[eventId]);
	}
	void entityEventTerminateAllOfType(EntityEvents::EventTypeID eventId);
	template <class T>
	void entityEventTerminateAllOfType() {
		entityEventTerminateAllOfType(EntityEvents::EventIDs<T>::ID);
	}


	void entityUpdate();
	void componentsUpdate();
};


#endif