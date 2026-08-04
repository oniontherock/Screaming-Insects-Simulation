#ifndef __MATH_H__
#define __MATH_H__

#include <algorithm>
#include <cmath>

template <typename T>
class Math {
public:
	static constexpr T PI = T(3.1415926535897932384626433832795028841971693993751058209749445923078164062862089986280348253421170679);
	static constexpr T TAU = T(6.283185307179586476925286766559005768394338798750216419498891846156328125724179972560696506842341359);

	inline static bool approxEquals(T a, T b, float epsilon) {
		return std::abs(a - b) < epsilon;
	}
	inline static int sign(T v) {
		return (T(0) < v) - (v < T(0));
	}

	inline static T clamp(T v, T min, T max) {
		return std::min(max, std::max(min, v));
	}
	inline static T lerp(T a, T b, T v) {
		T diff = fmod(b - a, TAU);
		T dist = fmod(2.0f * diff, TAU) - diff;
		return a + dist * v;
	}
	inline static T slerp(T a, T b, T v) {
		T diff = fmod(b - a, TAU);
		T dist = fmod(2.0f * diff, TAU) - diff;
		return a + dist * v;
	}

	inline static bool isEven(T v) {
		return (v % 2) == 0;
	}
	inline static bool isOdd(T v) {
		return (v % 2) != 0;
	}

	inline static float snap(T v, T s) {
		return std::round(v / s) * s;
	}

};

using Mathf = Math<float>;
using Mathd = Math<double>;

using Mathi8 = Math<int8_t>;
using Mathi16 = Math<int16_t>;
using Mathi32 = Math<int32_t>;
using Mathi64 = Math<int64_t>;
using Mathu8 = Math<uint8_t>;
using Mathu16 = Math<uint16_t>;
using Mathu32 = Math<uint32_t>;
using Mathu64 = Math<uint64_t>;


#endif