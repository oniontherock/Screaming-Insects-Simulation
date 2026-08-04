#include "LevelTypeDefinitions.hpp"




WorldPosition::WorldPosition() {
	level = LevelPosition(0, 0, 0);
	position = sf::Vector2f(0, 0);
}
WorldPosition::WorldPosition(LevelCoordinate _levelX, LevelCoordinate _levelY, LevelCoordinate _levelZ) {
	level = LevelPosition(_levelX, _levelY, _levelZ);
	position = sf::Vector2f(0, 0);
}
WorldPosition::WorldPosition(LevelPosition _level) {
	level = _level;
	position = sf::Vector2f(0, 0);
}
WorldPosition::WorldPosition(float _positionX, float _positionY) {
	level = LevelPosition(0, 0, 0);
	position = sf::Vector2f(_positionX, _positionY);
}
WorldPosition::WorldPosition(sf::Vector2f _position) {
	level = LevelPosition(0, 0, 0);
	position = _position;
}
WorldPosition::WorldPosition(LevelCoordinate _levelX, LevelCoordinate _levelY, LevelCoordinate _levelZ, float _positionX, float _positionY) {
	level = LevelPosition(_levelX, _levelY, _levelZ);
	position = sf::Vector2f(_positionX, _positionY);
}
WorldPosition::WorldPosition(LevelPosition _level, float _positionX, float _positionY) {
	level = _level;
	position = sf::Vector2f(_positionX, _positionY);
}
WorldPosition::WorldPosition(LevelCoordinate _levelX, LevelCoordinate _levelY, LevelCoordinate _levelZ, sf::Vector2f _position) {
	level = LevelPosition(_levelX, _levelY, _levelZ);
	position = _position;
}
WorldPosition::WorldPosition(LevelPosition _level, sf::Vector2f _position) {
	level = _level;
	position = _position;
}

// the position/id of the level.
LevelPosition level;
// the position in level.
sf::Vector2f position;

// sets the position to the specified position.
void WorldPosition::operator= (const sf::Vector2f& positionOther) {
	position = positionOther;
}
// sets the level to the specified level.
void WorldPosition::operator= (const LevelPosition& levelOther) {
	level = levelOther;
}
void WorldPosition::operator= (const WorldPosition& positionOther) {
	level = positionOther.level;
	position = positionOther.position;
}
// sets the position to the specified position.
void WorldPosition::operator= (sf::Vector2f& positionOther) {
	position = positionOther;
}
// sets the level to the specified level.
void WorldPosition::operator= (LevelPosition& levelOther) {
	level = levelOther;
}
void WorldPosition::operator= (WorldPosition& positionOther) {
	level = positionOther.level;
	position = positionOther.position;
}

bool WorldPosition::operator== (const WorldPosition& positionOther) {
	return level == positionOther.level && position == positionOther.position;
}

bool WorldPosition::operator== (WorldPosition& positionOther) {
	return level == positionOther.level && position == positionOther.position;
}




