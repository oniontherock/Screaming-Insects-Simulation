#ifndef __VECTOR_MATH_H__
#define __VECTOR_MATH_H__

#include "SFML/System.hpp"
#include <cmath>

template <typename T, typename R>
class Vector2Math {
private:
	typedef sf::Vector2<T> Vector2;
public:

	inline static R lengthSqrd(T x, T y) {
		return R((x * x) + (y * y));
	}

	inline static R lengthSqrd(Vector2 vec) {
		return lengthSqrd(vec.x, vec.y);
	}

	inline static R length(T x, T y) {
		return sqrt(lengthSqrd(x, y));
	}
	inline static R length(Vector2 vec) {
		return length(vec.x, vec.y);
	}

	// gets the axis from (aX, aY) to (bX, bY)
	inline static Vector2 axis(T aX, T aY, T bX, T bY) {
		return Vector2(bX - aX, bY - aY);
	}
	// gets the axis from vecA to vecB
	inline static Vector2 axis(Vector2 vecA, Vector2 vecB) {
		return axis(vecA.x, vecA.y, vecB.x, vecB.y);
	}

	inline static R distSqrd(T aX, T aY, T bX, T bY) {
		return lengthSqrd(axis(aX, aY, bX, bY));
	}
	inline static R distSqrd(Vector2 vecA, Vector2 vecB) {
		return distSqrd(vecA.x, vecA.y, vecB.x, vecB.y);
	}

	inline static R dist(T aX, T aY, T bX, T bY) {
		return length(axis(aX, aY, bX, bY));
	}
	inline static R dist(Vector2 vecA, Vector2 vecB) {
		return dist(vecA.x, vecA.y, vecB.x, vecB.y);
	}

	inline static Vector2 normalize(Vector2 vec) {
		return vec / length(vec);
	}
	inline static Vector2 normalize(T x, T y) {
		return Vector2(x, y) / length(x, y);
	}

	inline static Vector2 lengthSet(T x, T y, float len) {
		return normalize(x, y) * len;
	}
	inline static Vector2 lengthSet(Vector2 vec, float len) {
		return lengthSet(vec.x, vec.y, len);
	}


	inline static Vector2 lengthLimit(T x, T y, float limit) {
		return (lengthSqrd(x, y) > limit * limit) ? (normalize(x, y) * limit) : Vector2(x, y);
	}
	inline static Vector2 lengthLimit(Vector2 vec, float limit) {
		return lengthLimit(vec.x, vec.y, limit);
	}

	// gets the direction from (aX, aY) to (bX, bY)
	inline static Vector2 dir(T aX, T aY, T bX, T bY) {
		return normalize(axis(aX, aY, bX, bY));
	}
	// gets the direction from vecA to vecB
	inline static Vector2 dir(Vector2 vecA, Vector2 vecB) {
		return dir(vecA.x, vecA.y, vecB.x, vecB.y);
	}

	// returns the angle from (aX, aY) to (bX, bY)
	inline static R angle(T aX, T aY, T bX, T bY) {
		auto vecAxis = axis(aX, aY, bX, bY);

		return atan2(vecAxis.y, vecAxis.x);
	}
	// returns the angle from vecA to vecB
	inline static R angle(Vector2 vecA, Vector2 vecB) {
		return angle(vecA.x, vecA.y, vecB.x, vecB.y);
	}

	inline static R dot(T aX, T aY, T bX, T bY) {
		return (aX * bX) + (aY * bY);
	}
	inline static R dot(Vector2 vecA, Vector2 vecB) {
		return dot(vecA.x, vecA.y, vecB.x, vecB.y);
	}

	inline static R angleDifference(T aX, T aY, T bX, T bY) {

		R dotProduct = dot(aX, aY, bX, bY);
		R combinedLength = length(aX, aY) * length(bX, bY);

		return static_cast<R>(std::acos(dotProduct / combinedLength));
	}
	inline static R angleDifference(Vector2 vecA, Vector2 vecB) {
		return angleDifference(vecA.x, vecA.y, vecB.x, vecB.y);
	}

	inline static Vector2 reflect(T dX, T dY, T nX, T nY) {

		R dotProduct = dot(dX, dY, nX, nY);

		T rX = dX - (2 * dotProduct * nX);
		T rY = dY - (2 * dotProduct * nY);

		return Vector2(rX, rY);
	}
	inline static Vector2 reflect(Vector2 dir, Vector2 normal) {
		return reflect(dir.x, dir.y, normal.x, normal.y);
	}
	// rotates the given x and y by theta. Note that this adds instead of setting,
	// so calling this twice with PI as theta would result in the returned vector being the same as it was before, since it did a 360.
	template <typename F>
	inline static Vector2 rotate(T x, T y, F theta) {
		return Vector2(
			(x * T(std::cos(theta))) - (y * T(std::sin(theta))),
			(x * T(std::sin(theta))) + T(y * T(std::cos(theta)))
		);
	}
	// rotates the given vector by theta. Note that this adds instead of setting,
	// so calling this twice with PI as theta would result in the returned vector being the same as it was before, since it did a 360.
	template <typename F>
	inline static Vector2 rotate(Vector2 vector, F theta) {
		return rotate(vector.x, vector.y, theta);
	}
	inline static Vector2 abs(T x, T y) {
		return Vector2(std::abs(x), std::abs(y));
	}
	inline static Vector2 abs(Vector2 vector) {
		return abs(vector.x, vector.y);
	}


	// Given three collinear points p, q, r, the function checks if 
	// point q lies on line segment 'pr' 
	inline static bool onSegment(Vector2 p, Vector2 q, Vector2 r)
	{
		if (q.x <= std::max(p.x, r.x) && q.x >= std::min(p.x, r.x) &&
			q.y <= std::max(p.y, r.y) && q.y >= std::min(p.y, r.y))
			return true;

		return false;
	}
	// To find orientation of ordered triplet (p, q, r). 
	// The function returns following values 
	// 0 --> p, q and r are collinear 
	// 1 --> Clockwise 
	// 2 --> Counterclockwise 
	inline static uint16_t orientation(Vector2 p, Vector2 q, Vector2 r) {
		// for details of below formula. 
		T val = (q.y - p.y) * (r.x - q.x) -
			(q.x - p.x) * (r.y - q.y);

		if (val == 0) return 0;  // collinear 

		return (val > 0) ? 1 : 2; // clock or counterclock wise 
	}
	// The main function that returns true if line segment 'p1q1' 
	// and 'p2q2' intersect. 
	inline static bool linesIntersect(Vector2 p1, Vector2 q1, Vector2 p2, Vector2 q2)
	{
		// Find the four orientations needed for general and 
		// special cases 
		int o1 = orientation(p1, q1, p2);
		int o2 = orientation(p1, q1, q2);
		int o3 = orientation(p2, q2, p1);
		int o4 = orientation(p2, q2, q1);

		// General case 
		if (o1 != o2 && o3 != o4)
			return true;

		// Special Cases 
		// p1, q1 and p2 are collinear and p2 lies on segment p1q1 
		if (o1 == 0 && onSegment(p1, p2, q1)) return true;

		// p1, q1 and q2 are collinear and q2 lies on segment p1q1 
		if (o2 == 0 && onSegment(p1, q2, q1)) return true;

		// p2, q2 and p1 are collinear and p1 lies on segment p2q2 
		if (o3 == 0 && onSegment(p2, p1, q2)) return true;

		// p2, q2 and q1 are collinear and q1 lies on segment p2q2 
		if (o4 == 0 && onSegment(p2, q1, q2)) return true;

		return false; // Doesn't fall in any of the above cases 
	}
};

using Vector2fMath = Vector2Math<float, float>;
using Vector2dMath = Vector2Math<double, double>;
using Vector2iMath = Vector2Math<int, float>;

template <typename T, typename R>
class Vector3Math {
private:
	typedef sf::Vector2<T> Vector2;
	typedef sf::Vector3<T> Vector3;
public:
	inline static Vector3 cross(T aX, T aY, T aZ, T bX, T bY, T bZ) {

		Vector3 cross;

		cross.x = aY * bZ - aZ * bY;
		cross.y = aZ * bX - aX * bZ;
		cross.z = aX * bY - aY * bX;

		return cross;
	}
	inline static Vector3 cross(Vector3 vecA, Vector3 vecB) {
		return cross(vecA.x, vecA.y, vecA.z, vecB.x, vecB.y, vecB.z);
	}
	inline static Vector3 tripleProduct(T aX, T aY, T aZ, T bX, T bY, T bZ, T cX, T cY, T cZ) {
		Vector3 crossProduct = cross(aX, aY, aZ, bX, bY, bZ);

		return cross(crossProduct.x, crossProduct.y, crossProduct.z, cX, cY, cZ);
	}
	inline static Vector3 tripleProduct(Vector3 vecA, Vector3 vecB, Vector3 vecC) {
		return tripleProduct(vecA.x, vecA.y, vecA.z, vecB.x, vecB.y, vecB.z, vecC.x, vecC.y, vecC.z);
	}
	inline static Vector3 tripleProduct(Vector2 vecA, Vector2 vecB, Vector2 vecC) {
		return tripleProduct(vecA.x, vecA.y, 0, vecB.x, vecB.y, 0, vecC.x, vecC.y, 0);
	}

	inline static R dot(T aX, T aY, T aZ, T bX, T bY, T bZ) {
		return (aX * bX) + (aY * bY) + (aZ * bZ);
	}
	inline static R dot(Vector3 vecA, Vector3 vecB) {
		return dot(vecA.x, vecA.y, vecA.z, vecB.x, vecB.y, vecB.z);
	}
};

using Vector3fMath = Vector3Math<float, float>;
using Vector3dMath = Vector3Math<double, double>;
using Vector3iMath = Vector3Math<int, float>;


#endif