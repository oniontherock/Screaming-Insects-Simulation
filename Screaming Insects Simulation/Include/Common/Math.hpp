#ifndef __MATH_H__
#define __MATH_H__

#include "SFML/System.hpp"

template <typename T>
class Math {
public:
	static constexpr T PI = T(3.1415926535897932384626433832795028841971693993751058209749445923078164062862089986280348253421170679);
	static constexpr T TAU = T(6.283185307179586476925286766559005768394338798750216419498891846156328125724179972560696506842341359);
};

using Mathf = Math<float>;
using Mathd = Math<double>;

template <typename T>
class Vector2Math {
public:

	static float lengthSqrd(T x, T y) {
		return (x * x) + (y * y);
	}

	static float lengthSqrd(sf::Vector2<T> vec) {
		return lengthSqrd(vec.x, vec.y);
	}

	static float length(T x, T y) {
		return sqrt(lengthSqrd(x, y));
	}
	static float length(sf::Vector2<T> vec) {
		return length(vec.x, vec.y);
	}

	static sf::Vector2<T> axis(T aX, T aY, T bX, T bY) {
		return sf::Vector2<T>(aX - bX, aY - bY);
	}
	static sf::Vector2<T> axis(sf::Vector2<T> vecA, sf::Vector2<T> vecB) {
		return axis(vecA.x, vecA.y, vecB.x, vecB.y);
	}

	static float distSqrd(T aX, T aY, T bX, T bY) {
		return lengthSqrd(axis(aX, aY, bX, bY));
	}
	static float distSqrd(sf::Vector2<T> vecA, sf::Vector2<T> vecB) {
		return distSqrd(vecA.x, vecA.y, vecB.x, vecB.y);
	}

	static float dist(T aX, T aY, T bX, T bY) {
		return length(axis(aX, aY, bX, bY));
	}
	static float dist(sf::Vector2<T> vecA, sf::Vector2<T> vecB) {
		return dist(vecA.x, vecA.y, vecB.x, vecB.y);
	}

	static sf::Vector2<T> normalize(sf::Vector2<T> vec) {
		return vec / length(vec);
	}
	static sf::Vector2<T> normalize(T x, T y) {
		return sf::Vector2<T>(x, y) / length(x, y);
	}

	static sf::Vector2<T> dir(T aX, T aY, T bX, T bY) {
		return normalize(axis(aX, aY, bX, bY));
	}
	static sf::Vector2<T> dir(sf::Vector2<T> vecA, sf::Vector2<T> vecB) {
		return dir(vecA.x, vecA.y, vecB.x, vecB.y);
	}


	static float angle(T aX, T aY, T bX, T bY) {
		auto vecAxis = axis(aX, aY, bX, bY);

		return atan2(vecAxis.y, vecAxis.x);
	}
	static float angle(sf::Vector2<T> vecA, sf::Vector2<T> vecB) {
		return angle(vecA.x, vecA.y, vecB.x, vecB.y);
	}

	static float dot(T aX, T aY, T bX, T bY) {
		return (aX * bX) + (aY * bY);
	}
	static float dot(sf::Vector2<T> vecA, sf::Vector2<T> vecB) {
		return dot(vecA.x, vecA.y, vecB.x, vecB.y);
	}

	static sf::Vector2<T> reflect(T dX, T dY, T nX, T nY) {

		float dotProduct = dot(dX, dY, nX, nY);

		T rX = dX - (2 * dotProduct * nX);
		T rY = dY - (2 * dotProduct * nY);

		return sf::Vector2<T>(rX, rY);
	}
	static sf::Vector2<T> reflect(sf::Vector2<T> dir, sf::Vector2<T> normal) {
		return reflect(dir.x, dir.y, normal.x, normal.y);
	}
};

using Vector2fMath = Vector2Math<float>;
using Vector2iMath = Vector2Math<int>;

#endif