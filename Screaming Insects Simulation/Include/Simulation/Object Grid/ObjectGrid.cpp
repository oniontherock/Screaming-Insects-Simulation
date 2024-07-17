#include "ObjectGrid.hpp"


void Cell::setType(TargetType type, bool state) {
	typesUSet.set(type, state);
}
bool Cell::hasType(TargetType type) {
	return typesUSet[type];
}


CellGrid ObjectGrid::cellGrid;

void ObjectGrid::gridInitialize(CellCoordinate width, CellCoordinate height) {
	cellGrid = Cells2D(width, Cells1D(height));
}

Cell& ObjectGrid::gridGetCell(CellCoordinate x, CellCoordinate y) {
	return cellGrid[x][y];
}

