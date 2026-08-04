#include "EventIDs.hpp"

// the maximum value for any ID
EntityEvents::EventTypeID EntityEvents::maxID = 0;
// the total amount of components
EntityEvents::EventTypeID EntityEvents::totalEventTypes = 0;

std::vector<EntityEvents::Event*> EntityEvents::allEvents;

void EntityEvents::allEventsTerminate() {
	for (uint16_t i = 0; i < EntityEvents::allEvents.size(); i++) {
		delete EntityEvents::allEvents[i];
	}
}
