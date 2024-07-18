#ifndef __OBJECT_GRID_H__
#define __OBJECT_GRID_H__

#include <iostream>
#include <cstdint>
#include <unordered_set>
#include <set>
#include <bitset>

#include "SFML/System/Vector2.hpp"
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
	// returns whether the entity has any type at all
	bool hasAnyType();
	// returns the first active type the cell contains
	TargetType getFirstType();
};

using Cells1D = std::vector<Cell>;
using Cells2D = std::vector<Cells1D>;

typedef Cells2D CellGrid;

using CellCoordinate = uint16_t;
using CellDimensions = sf::Vector2i;
using GridDimensions = sf::Vector2i;

class ObjectGrid {
	static CellGrid cellGrid;
public:

	static CellDimensions cellDimensions;

	static sf::Vector2i coordinatesRealToCell(float realX, float realY);
	static sf::Vector2i coordinatesRealToCell(sf::Vector2f realVec);

	static sf::Vector2f coordinatesCellToReal(CellCoordinate cellX, CellCoordinate cellY);
	static sf::Vector2f coordinatesCellToReal(sf::Vector2i cellVec);

	static void gridInitialize(CellDimensions _cellDimensions, CellCoordinate width, CellCoordinate height);

	static Cell& gridGetCell(CellCoordinate x, CellCoordinate y);
	static Cell& gridGetCellFromReal(float realX, float realY);

	static GridDimensions gridGetDimensions();
};

#endif