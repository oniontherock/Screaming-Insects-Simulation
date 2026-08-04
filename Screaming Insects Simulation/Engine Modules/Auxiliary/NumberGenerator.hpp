#ifndef __NUMBER_GENERATOR_H__
#define __NUMBER_GENERATOR_H__

#include <random>

class NumberGenerator {
protected:
	// used to seed generator
	std::random_device generatorSeed;

	uint32_t getState() {
		uint32_t x = state;

		x ^= x << 13;
		x ^= x >> 17;
		x ^= x << 5;

		return state = x;
	}

private:
	// random number generator
	uint32_t state = generatorSeed();
};

template <typename T>
class RealNumberGenerator : public NumberGenerator {
public:

	RealNumberGenerator() :
		NumberGenerator()
	{}

	float get() {
		return float(getState()) / UINT32_MAX;
	}

	float getUnder(T max) {
		return get() * max;
	}

	float getRange(T min, T max) {
		return min + get() * (max - min);
	}

	float getRange(T width) {
		return getRange(-width * 0.5f, width * 0.5f);
	}
};

template <typename T>
class RNG {
private:
	static RealNumberGenerator<T>* generator;
public:

	static void initialize() {
		generator = new RealNumberGenerator<T>();
	}

	static T get() {
		return generator->get();
	}

	static float getUnder(T max) {
		return generator->getUnder(max);
	}

	static float getRange(T min, T max) {
		return generator->getRange(min, max);
	}

	static float getRange(T width) {
		return generator->getRange(width);
	}

	static float getFullRange(T width) {
		return generator->getRange(static_cast<T>(2.f) * width);
	}

	static bool probability(float value) {
		return value > get();
	}
};

using RNGf = RNG<float>;

template <typename T>
RealNumberGenerator<T>* RNG<T>::generator = nullptr;

template <typename T>
class IntegerNumberGenerator : public NumberGenerator {
public:
	IntegerNumberGenerator() :
		NumberGenerator()
	{}

	T getUnder(T max) {

		return getState() % max;
	}

	T getRange(T min, T max) {
		return (getState() % (max - min)) + min;
	}
};

template <typename T>
class RNGi {
private:
	static IntegerNumberGenerator<T> generator;

public:
	static T getUnder(T max) {
		return generator.getUnder(max);
	}

	static T getRange(T min, T max) {
		return generator.getRange(min, max);
	}
};

template <typename T>

IntegerNumberGenerator<T> RNGi<T>::generator;

using RNGi8 = RNGi<int8_t>;
using RNGi16 = RNGi<int16_t>;
using RNGi32 = RNGi<int32_t>;
using RNGi64 = RNGi<int64_t>;
using RNGu8 = RNGi<uint8_t>;
using RNGu16 = RNGi<uint16_t>;
using RNGu32 = RNGi<uint32_t>;
using RNGu64 = RNGi<uint64_t>;

template <typename T>
class RNGPool {
private:
	static inline std::vector<std::vector<T>> pools;
	static inline std::vector<uint32_t> poolValueIndexes;
	static inline uint16_t poolCount = 0;

	static void poolIndexIncrement(uint16_t poolIndex) {
		if ((++poolValueIndexes[poolIndex]) >= pools[poolIndex].size()) poolValueIndexes[poolIndex] = 0;
	}
	static uint32_t poolIndexIncrementAndGet(uint16_t poolIndex) {
		poolIndexIncrement(poolIndex);
		return poolValueIndexes[poolIndex];
	}
public:
	// creates a new RNG pool and returns it's pool index
	static uint16_t poolCreate(uint32_t size) {
		pools.push_back(std::vector<T>(size, T(0)));
		poolValueIndexes.push_back(0);
		return poolCount++;
	}

	static void poolValueSetAtIndex(uint16_t poolIndex, uint32_t valueIndex, T value) {
		pools[poolIndex][valueIndex] = value;
	}

	static void poolFill(uint16_t poolIndex) {
		for (uint32_t i = 0; i < pools[poolIndex].size(); i++) {
			pools[poolIndex][i] = RNG<T>::get();
		}
	}
	static void poolFillRange(uint16_t poolIndex, T width) {
		for (uint32_t i = 0; i < pools[poolIndex].size(); i++) {
			pools[poolIndex][i] = RNG<T>::getRange(width);
		}
	}
	static void poolFillRange(uint16_t poolIndex, T min, T max) {
		for (uint32_t i = 0; i < pools[poolIndex].size(); i++) {
			pools[poolIndex][i] = RNG<T>::getRange(min, max);
		}
	}
	static T poolValueGet(uint16_t poolIndex) {
		return pools[poolIndex][poolIndexIncrementAndGet(poolIndex)];
	}
};
template <typename T>
class RNGiPool {
private:
	static inline std::vector<std::vector<T>> pools;
	static inline std::vector<uint32_t> poolValueIndexes;
	static inline uint16_t poolCount = 0;

	static void poolIndexIncrement(uint16_t poolIndex) {
		if ((++poolValueIndexes[poolIndex]) >= pools[poolIndex].size()) poolValueIndexes[poolIndex] = 0;
	}
	static uint32_t poolIndexIncrementAndGet(uint16_t poolIndex) {
		poolIndexIncrement(poolIndex);
		return poolValueIndexes[poolIndex];
	}
public:
	// creates a new RNG pool and returns it's pool index
	static uint16_t poolCreate(uint32_t size) {
		pools.push_back(std::vector<T>(size, T(0)));
		poolValueIndexes.push_back(0);
		return poolCount++;
	}

	static void poolValueSetAtIndex(uint16_t poolIndex, uint32_t valueIndex, T value) {
		pools[poolIndex][valueIndex] = value;
	}

	static void poolFill(uint16_t poolIndex) {
		for (uint32_t i = 0; i < pools[poolIndex].size(); i++) {
			pools[poolIndex][i] = RNGi<T>::get();
		}
	}
	static void poolFillRange(uint16_t poolIndex, T width) {
		for (uint32_t i = 0; i < pools[poolIndex].size(); i++) {
			pools[poolIndex][i] = RNGi<T>::getRange(width);
		}
	}
	static void poolFillRange(uint16_t poolIndex, T min, T max) {
		for (uint32_t i = 0; i < pools[poolIndex].size(); i++) {
			pools[poolIndex][i] = RNGi<T>::getRange(min, max);
		}
	}
	static T poolValueGet(uint16_t poolIndex) {
		return pools[poolIndex][poolIndexIncrementAndGet(poolIndex)];
	}
};

using RNGfPool = RNGPool<float>;
using RNGdPool = RNGPool<double>;

using RNGi8Pool = RNGiPool<int8_t>;
using RNGi16Pool = RNGiPool<int16_t>;
using RNGi32Pool = RNGiPool<int32_t>;
using RNGi64Pool = RNGiPool<int64_t>;

using RNGu8Pool = RNGiPool<uint8_t>;
using RNGu16Pool = RNGiPool<uint16_t>;
using RNGu32Pool = RNGiPool<uint32_t>;
using RNGu64Pool = RNGiPool<uint64_t>;

#endif