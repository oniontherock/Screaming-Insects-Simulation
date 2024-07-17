#ifndef __OBJECT_GRID_H__
#define __OBJECT_GRID_H__

#include <cstdint>
#include <unordered_set>
#include <set>
#include <bitset>
#include "../TargetTypes.hpp"

// cell that has a set of types it contains
class Cell {
	// bitset of types in this set,
	// false means the set doesn't have the type, true means it does
	std::bitset<TargetType::TypesCount> typesUSet;
public:
	// set a type in the typesUSet to the specified state
	void setType(TargetType type, bool state);
	// check if the typesUSet has the type
	bool hasType(TargetType type);
	// clears the typesUSet
	void clearTypes();
};

using Cells1D = std::vector<Cell>;
using Cells2D = std::vector<Cells1D>;

typedef Cells2D CellGrid;

using CellCoordinate = uint16_t;

class ObjectGrid {
	static CellGrid cellGrid;
public:
	static void gridInitialize(CellCoordinate width, CellCoordinate height);
	static Cell& gridGetCell(CellCoordinate x, CellCoordinate y);
};

#endif