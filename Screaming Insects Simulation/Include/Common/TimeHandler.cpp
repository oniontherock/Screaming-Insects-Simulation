#include "DeltaManager.hpp"


uint32_t TimeHandler::timeRealPrev{ 0 };
uint32_t TimeHandler::timeRealCur{ 0 };
uint32_t TimeHandler::timeSimulatedPrev{ 0 };
uint32_t TimeHandler::timeSimulatedCur{ 0 };
double TimeHandler::deltaReal{ 0 };
double TimeHandler::deltaSimulated{ 0 };

void TimeHandler::deltaCompute() {

	using namespace std::chrono;

	timeRealCur = uint32_t(duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count());
	uint32_t timeDiffReal = timeRealCur - timeRealPrev;
	timeRealPrev = timeRealCur;

	deltaReal = float(timeDiffReal) / 1000.f;

	// the simulated time movement in milliseconds since last frame.
	// timeSimulatedCur + simulatedTimeMove is equal to the current time according to the timescale
	float simulatedTimeMovement = (timeDiffReal * (timescale * 1000)) / 1000;

	timeSimulatedCur += simulatedTimeMovement;
	float timeDiffSimulated = timeSimulatedCur - timeSimulatedPrev;
	timeSimulatedPrev = timeRealCur;

	deltaSimulated = float(timeDiffSimulated) / 1000.f;
}
double TimeHandler::deltaRealGet() {
	return deltaReal;
}
double TimeHandler::deltaSimulatedGet() {
	return deltaSimulated;
}

uint32_t TimeHandler::timeRealGet() {
	return timeRealCur;
}
uint32_t TimeHandler::timeSimulatedGet() {
	return timeSimulatedCur;

}
