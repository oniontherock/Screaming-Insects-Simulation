#ifndef __EVENT_H__
#define __EVENT_H__

#include "../Duplicatable/Duplicatable.hpp"
#include <iostream>

namespace EntityEvents {

	struct Event : public Duplicatable {
		// resets all the event's data to it's default values
		virtual void clear() = 0;
	};

	// used for initializing certain components
	struct EventInitialize final : public Event {

		EventInitialize() {};

		void clear() final {
		}
		std::unique_ptr<Duplicatable> duplicate() override {
			return std::unique_ptr<Duplicatable>(new EventInitialize());
		};
	};

	void eventIDsInitialize();
}

typedef std::unique_ptr<EntityEvents::Event> EventUniquePtr;
typedef std::shared_ptr<EntityEvents::Event> EventSharedPtr;

#endif