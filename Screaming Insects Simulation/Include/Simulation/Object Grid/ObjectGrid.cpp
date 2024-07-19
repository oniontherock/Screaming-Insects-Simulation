#include "ObjectGrid.hpp"


void Cell::setType(TargetType type, bool state) {
	typesUSet.set(type, state);
}
bool Cell::hasType(TargetType type) {
	return typesUSet[type];
}
void Cell::clearTypes() {
	typesUSet.reset();
}
bool Cell::hasAnyType() {
	return typesUSet.any();
}
TargetType Cell::getFirstType() {
	for (uint16_t typeCur = 0; typeCur < TargetType::TypesCount; typeCur++) {
		if (typesUSet[typeCur]) {
			return static_cast<TargetType>(typeCur);
		}
	}
	return TargetType::TypesCount;
}


CellGrid ObjectGrid::cellGrid{};
CellDimensions ObjectGrid::cellDimensions = CellDimensions(0, 0);


sf::Vector2i ObjectGrid::coordinatesRealToCell(float realX, float realY) {
	return sf::Vector2i(CellCoordinate(realX / cellDimensions.x), CellCoordinate(realY / cellDimensions.y));
}
sf::Vector2i ObjectGrid::coordinatesRealToCell(sf::Vector2f realVec) {
	return coordinatesRealToCell(realVec.x, realVec.y);
}

sf::Vector2f ObjectGrid::coordinatesCellToReal(CellCoordinate cellX, CellCoordinate cellY) {
	return sf::Vector2f(CellCoordinate(cellX * cellDimensions.x), CellCoordinate(cellY * cellDimensions.y));
}
sf::Vector2f ObjectGrid::coordinatesCellToReal(sf::Vector2i cellVec) {
	return coordinatesCellToReal(CellCoordinate(cellVec.x), CellCoordinate(cellVec.y));
}

void ObjectGrid::gridInitialize(CellDimensions _cellDimensions, CellCoordinate width, CellCoordinate height) {
	cellDimensions = _cellDimensions;

	cellGrid = Cells2D(width, Cells1D(height));
}

Cell& ObjectGrid::gridGetCell(CellCoordinate x, CellCoordinate y) {
	return cellGrid[x][y];
}
Cell& ObjectGrid::gridGetCellFromReal(float realX, float realY) {
	return cellGrid[CellCoordinate(realX / cellDimensions.x)][CellCoordinate(realY / cellDimensions.y)];
}

GridDimensions ObjectGrid::gridGetDimensions() {
	return GridDimensions(int(cellGrid.size()), int(cellGrid[0].size()));
}

void ObjectGrid::gridCellsClear() {
	for (uint16_t x = 0; x < cellGrid.size(); x++) {
		for (uint16_t y = 0; y < cellGrid[x].size(); y++) {
			cellGrid[x][y].clearTypes();
		}
	}
}

