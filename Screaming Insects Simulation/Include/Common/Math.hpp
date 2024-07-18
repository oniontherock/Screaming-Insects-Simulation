#ifndef __MATH_H__
#define __MATH_H__

#include "SFML/System.hpp"

template <typename T>
class Math {
public:
	static constexpr T PI = 3.1415926535897932384626433832795028841971693993751058209749445923078164062862089986280348253421170679;
};

using Mathf = Math<float>;
using Mathd = Math<double>;

template <typename T>
class Vector2Math {
public:
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