#include "Entity.hpp"
#include "EntityManager.hpp"

Entity::Entity() {
	updateType = EntityUpdateType::Frame;
	Id = 0;
}
Entity::Entity(EntityId _id, EntityUpdateType _updateType) {
	Id = _id;
	updateType = _updateType;

	componentsInitialize();
	eventsInitialize();
}
Entity::Entity(EntityId _id, EntityUpdateType _updateType, LevelPosition _levelId) {
	Id = _id;
	updateType = _updateType;
	levelId = _levelId;

	componentsInitialize();
	eventsInitialize();
}

template <class T>
static void copyUniquePtrVector(std::vector<std::unique_ptr<T>>& A, const std::vector<std::unique_ptr<T>>& B) {
	A.clear();
	for (uint16_t i = 0; i < B.size(); i++) {
		if (!static_cast<bool>(B[i])) {
			A.push_back(std::unique_ptr<T>(nullptr));
			continue;
		}
		A.push_back(Duplicatable::duplicateAndConvertToType<T>(B[i].get()));
	}
}

Entity::Entity(Entity& other) {
	Id = other.Id;
	updateType = other.updateType;
	levelId = other.levelId;

	copyUniquePtrVector<EntityComponents::Component>(componentsVector, other.componentsVector);
	eventsInitialize();
	for (uint16_t i = 0; i < eventsVector.size(); i++) {
		copyUniquePtrVector<EntityEvents::Event>(eventsVector[i], other.eventsVector[i]);
	}
}
Entity& Entity::operator= (const Entity& other) {

	Id = other.Id;
	updateType = other.updateType;
	levelId = other.levelId;

	copyUniquePtrVector<EntityComponents::Component>(componentsVector, other.componentsVector);
	eventsInitialize();
	for (uint16_t i = 0; i < eventsVector.size(); i++) {
		copyUniquePtrVector<EntityEvents::Event>(eventsVector[i], other.eventsVector[i]);
	}

	return *this;
}
Entity::~Entity() {}

void Entity::componentsInitialize() {
	componentsVector = std::vector<ComponentUniquePtr>(EntityComponents::totalComponents);
};
void Entity::eventsInitialize() {

	std::vector<EventVector> columns(MAX_EVENT_TYPES);
	for (uint16_t i = 0; i < columns.size(); i++) {
		columns[i] = EventVector();
	}
	eventsVector = std::move(columns);
};

bool Entity::entityComponentHasAtIndex(EntityComponents::ComponentTypeID index) {
	return static_cast<bool>(componentsVector[index]);
}
EntityComponents::Component* Entity::entityComponentGetAtIndex(EntityComponents::ComponentTypeID index) {
	return componentsVector[index].get();
}
void Entity::entityComponentAddAtIndex(EntityComponents::Component* component, EntityComponents::ComponentTypeID index) {
	componentsVector[index] = ComponentUniquePtr(component);
}
void Entity::entityComponentAddAtIndexNoOverwrite(EntityComponents::Component* component, EntityComponents::ComponentTypeID index) {
	if (entityComponentHasAtIndex(index)) return;

	entityComponentAddAtIndex(component, index);
}

void Entity::entityUpdate() {
	componentsUpdate();
	// deactivating all events is basically just clearing the entity's events
	if (hasAnyEvent) eventsAllTerminate();
}
void Entity::componentsUpdate() {
	for (uint16_t i = 0; i < EntityComponents::totalComponents; i++) {

		if (componentsVector[i] == nullptr) continue;
		if (!static_cast<bool>(componentsVector[i])) continue;
		if (!componentsVector[i]->hasSystem) continue;

		componentsVector[i]->system(*this);
	}
}

void Entity::entityEventTerminate(EntityEvents::EventTypeID eventId, uint16_t ind) {
	EventPool::eventPoolGive(std::move(eventsVector[eventId][ind]), eventId);

	eventsVector[eventId].erase(eventsVector[eventId].begin() + ind);
}

void Entity::componentsAllTerminate() {
	for (EntityComponents::ComponentTypeID i = 0; i < EntityComponents::totalComponents; i++) {
		componentsVector[i].reset();
	}
}

void Entity::entityEventTerminateAllOfType(EntityEvents::EventTypeID eventId) {
	for (int32_t i = int32_t(eventsVector[eventId].size() - 1); i >= 0; i--) {
		entityEventTerminate(eventId, i);
	}
}

void Entity::eventsAllTerminate() {

	for (EntityEvents::EventTypeID entityTypeInd = 0; entityTypeInd < EntityEvents::totalEventTypes; entityTypeInd++) {
		entityEventTerminateAllOfType(entityTypeInd);
	}

	hasAnyEvent = false;
}

void Entity::terminate() {
	componentsAllTerminate();
	eventsAllTerminate();
}