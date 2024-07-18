#ifndef __TARGET_TYPES_H__
#define __TARGET_TYPES_H__

#include <cstdint>
#include <unordered_map>

enum TargetType : uint8_t {
	Home,
	Food,
	TypesCount, // amount of types in the TargetType enum, not actually a TargetType
};
// amount of steps to a certain target
typedef uint16_t Steps;

typedef std::pair<TargetType, Steps> TargetTypeStepPair;

// vector of steps from certain target types,
// the indexes in this vector correspond to their indexes in the TargetType enum.
typedef std::vector<Steps> TargetStepsVector;

#endif