#include "EventPool.hpp"
#include "../TypeDefinitions.hpp"


std::vector<std::vector<EventUniquePtr>> EventPool::eventPool{};

void EventPool::eventPoolInitialize() {

	using Pool1D = std::vector<EventUniquePtr>;
	using Pool2D = std::vector<Pool1D>;

	Pool2D columns(MAX_EVENT_TYPES);
	for (uint16_t x = 0; x < MAX_EVENT_TYPES; x++) {
		columns[x] = Pool1D();
	}
	eventPool = std::move(columns);
}

