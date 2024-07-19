#ifndef __RANDOM_POOLS_H__
#define __RANDOM_POOLS_H__

#include "NumberGenerator.hpp"
#include "Math.hpp"


#define ANGLE_POOL_IND 0
#define ANGLE_POOL_SIZE 1000


inline void poolsInitialize() {
	RNGfPool::createNewPool(ANGLE_POOL_SIZE);
	RNGfPool::fillPoolRange(ANGLE_POOL_IND, -Mathf::PI / 32.f, +Mathf::PI / 32.f);
}

#endif