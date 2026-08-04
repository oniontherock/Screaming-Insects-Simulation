#ifndef __LEVEL_TYPE_DEFINITIONS_H__
#define __LEVEL_TYPE_DEFINITIONS_H__

#include "SFML/System/Vector3.hpp"
#include "SFML/System/Vector2.hpp"
#include <cstdint>

// position of a level in the GameWorld's levelGrid
using LevelPosition = sf::Vector3<uint32_t>;

// level position representing no level position (for entities that aren't in a room, I.E. intangible)
#define NoLevelPosition LevelPosition(UINT32_MAX, UINT32_MAX, UINT32_MAX)

typedef uint32_t LevelCoordinate;

// a position in the world, contains a levelPosition, and a position in that level.
struct WorldPosition {

	WorldPosition();
	WorldPosition(LevelCoordinate _levelX, LevelCoordinate _levelY, LevelCoordinate _levelZ);
	WorldPosition(LevelPosition _level);
	WorldPosition(float _positionX, float _positionY);
	WorldPosition(sf::Vector2f _position);
	WorldPosition(LevelCoordinate _levelX, LevelCoordinate _levelY, LevelCoordinate _levelZ, float _positionX, float _positionY);
	WorldPosition(LevelPosition _level, float _positionX, float _positionY);
	WorldPosition(LevelCoordinate _levelX, LevelCoordinate _levelY, LevelCoordinate _levelZ, sf::Vector2f _position);
	WorldPosition(LevelPosition _level, sf::Vector2f _position);

	// the position/id of the level.
	LevelPosition level;
	// the position in level.
	sf::Vector2f position;

	// sets the position to the specified position.
	void operator= (const sf::Vector2f& positionOther);
	// sets the level to the specified level.
	void operator= (const LevelPosition& levelOther);
	void operator= (const WorldPosition& positionOther);
	// sets the position to the specified position.
	void operator= (sf::Vector2f& positionOther);
	// sets the level to the specified level.
	void operator= (LevelPosition& levelOther);
	void operator= (WorldPosition& positionOther);

	bool operator== (const WorldPosition& positionOther);
	bool operator== (WorldPosition& positionOther);


};


#endif
