#ifndef __GAME_LEVEL_H__
#define __GAME_LEVEL_H__

#include "World/Level.hpp"
#include "SFML/System/Vector2.hpp"
#include "../Include/Simulation/TargetTypes.hpp"

typedef std::tuple<sf::Vector2f, sf::Vector2f, TargetType> ScreamConnection;

// example of the game level class,
// 
// note that there should generally only be one type of GameLevel,
// the reason GameLevel inherits BaseLevel and needs to be down-casted is so the user can define their own variables inside of GameLevel.
// the GameLevel and BaseLevel relationship isn't like the Component relationship where there can be many types of components,
struct GameLevel : public BaseLevel {

	GameLevel();
	GameLevel(LevelCoordinate _idX, LevelCoordinate _idY, LevelCoordinate _idZ);
	GameLevel(LevelPosition _id);

	sf::Vector2i levelSize;

	bool firstRun = true;

	std::vector<EntityId> targets;

	std::vector<EntityId> insects;
	std::vector<std::vector<std::vector<EntityId>>> hearingGrid;

	std::vector<ScreamConnection> screamConnections;
	// screams that an entity has turned towards
	std::vector<ScreamConnection> acceptedScreamConnections;

	sf::Vector2i coordinateRealToHearing(float realX, float realY);
};

#endif