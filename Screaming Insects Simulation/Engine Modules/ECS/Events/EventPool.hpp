#ifndef __EVENT_POOL_H__
#define __EVENT_POOL_H__

#include "Event.hpp"
#include "EventIDs.hpp"

class EventPool {
	static std::vector<std::vector<EventUniquePtr>> eventPool;

public:
	static void eventPoolInitialize();
	template <class T>
	static inline EventUniquePtr eventPoolTake() {

		EntityEvents::EventTypeID eventId = EntityEvents::EventIDs<T>::ID;

		if (eventPool[eventId].size() == 0) {
			eventPoolGive(EventUniquePtr(new T()), eventId);
		}

		EventUniquePtr item = std::move(eventPool[eventId].back());
		eventPool[eventId].pop_back();

		assert(dynamic_cast<T*>(item.get()) != nullptr && "pool slot holds wrong type");

		item->clear();

		return std::move(item);
	}
	static inline void eventPoolGive(EventUniquePtr item, EntityEvents::EventTypeID eventId) {
		eventPool[eventId].push_back(std::move(item));
	}
	template <class T>
	static inline void eventPoolGive(EventUniquePtr item) {
		eventPoolGive(std::move(item), EntityEvents::EventIDs<T>::ID);
	}
};

#endif