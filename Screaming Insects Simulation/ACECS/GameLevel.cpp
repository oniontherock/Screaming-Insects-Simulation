#include "GameLevel.hpp"

// this file is used for defining functions of the GameLevel class

#include "ECSRegistry.hpp"

GameLevel::GameLevel() {
	entities = std::vector<EntityId>();

	levelSize = sf::Vector2i(1280, 720);

	using InsectGrid1D = std::vector<EntityId>;
	using InsectGrid2D = std::vector<InsectGrid1D>;
	using InsectGrid3D = std::vector<InsectGrid2D>;

	float screamRad = EntityComponents::ComponentScream::MAX_SCREAM_DIST;

	hearingGrid = InsectGrid3D(levelSize.x / uint64_t(screamRad), InsectGrid2D(levelSize.y / uint64_t(screamRad), InsectGrid1D(0)));
}

GameLevel::GameLevel(LevelCoordinate _idX, LevelCoordinate _idY, LevelCoordinate _idZ) :
	GameLevel()
{
	levelPosition = LevelPosition(_idX, _idY, _idZ);
}

GameLevel::GameLevel(LevelPosition _id) :
	GameLevel(_id.x, _id.y, _id.z)
{}

sf::Vector2i GameLevel::coordinateRealToHearing(float realX, float realY) {
	float halfScreamDist = EntityComponents::ComponentScream::MAX_SCREAM_DIST / 2.f;

	float correctedX = realX - halfScreamDist;
	float correctedY = realY - halfScreamDist;

	uint16_t gridX = uint16_t(std::clamp<int32_t>(
		int32_t(std::round(correctedX / EntityComponents::ComponentScream::MAX_SCREAM_DIST)),
		0,
		int32_t(hearingGrid.size() - 1)
	));

	uint16_t gridY = uint16_t(std::clamp<int32_t>(
		int32_t(std::roundf(correctedY / EntityComponents::ComponentScream::MAX_SCREAM_DIST)),
		0,
		int32_t(hearingGrid[gridX].size() - 1)
	));

	return sf::Vector2i(gridX, gridY);
}
