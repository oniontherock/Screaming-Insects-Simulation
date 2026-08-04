#include "GameStates.hpp"

#include "../Include/Common/TimeHandler.hpp"
#include "../Include/Simulation/Object Grid/ObjectGrid.hpp"
#include "ECS/Entities/EntityManager.hpp"
#include "ECSRegistry.hpp"
#include "GameLevel.hpp"
#include "Graphics.hpp"
#include "Panels.hpp"
#include "World/LevelUpdater.hpp"


void GameStatePlay::gameStateUpdate() {

	GameLevel* gameLevel = GameLevelGrid::levelGet(0, 0, 0);

	if (gameLevel->firstRun) {
		gameLevel->firstRun = false;
		gameStateFirstStart();
	}

	if ((!InputInterface::inputGetActive("Spawn Home")) != (!InputInterface::inputGetActive("Spawn Food"))) {
		EntityId entityId = EntityManager::entityCreate(0, 0, 0, "Target", EntityUpdateType::Frame);

		Entity& entityInstance = EntityManager::entitiesVector[entityId];

		auto* entityPositionComponent = entityInstance.entityComponentGet<EntityComponents::ComponentPosition>();

		sf::Vector2f panelMousePosition = PanelManager::panelGet(PanelTypes::GameView).viewMousePositionGet();

		entityPositionComponent->x = panelMousePosition.x;
		entityPositionComponent->y = panelMousePosition.y;

		float populationRadius = 8 * 4;

		auto* gridCellPopulatorComponent = entityInstance.entityComponentGet<EntityComponents::ComponentObjectGridCellPopulator>();
		gridCellPopulatorComponent->popType = InputInterface::inputGetActive("Spawn Home") ? TargetType::Home : TargetType::Food;
		gridCellPopulatorComponent->popRadius = populationRadius;

		auto* gridCellDepopulatorComponent = entityInstance.entityComponentGet<EntityComponents::ComponentObjectGridCellDepopulator>();
		gridCellDepopulatorComponent->popRadius = populationRadius;


		entityInstance.entityComponentGet<EntityComponents::ComponentRotation>()->rotation = RNGf::getFullRange(Mathf::PI);

		gameLevel->targets.push_back(entityId);
	}

	if (InputInterface::inputGetActive("Remove Target")) {

		for (uint32_t i = 0; i < gameLevel->targets.size(); i++) {
			
			Entity& entityCur = EntityManager::entitiesVector[gameLevel->targets[i]];

			auto* entityPositionComponent = entityCur.entityComponentGet<EntityComponents::ComponentPosition>();

			sf::Vector2f mousePos = PanelManager::panelGet(PanelName::GameView).viewMousePositionGet();

			if (Vector2fMath::distSqrd(mousePos.x, mousePos.y, entityPositionComponent->x, entityPositionComponent->y) < 32 * 32) {
				entityCur.entityComponentGet<EntityComponents::ComponentObjectGridCellDepopulator>()->system(entityCur);
				EntityManager::entityTerminate(gameLevel->targets[i]);
				gameLevel->targets.erase(gameLevel->targets.begin() + i);
				break;
			}
		}
	}

	if (InputInterface::inputGetActive("Increase Screaming Range")) {
		EntityComponents::ComponentScream::MAX_SCREAM_DIST += 4;
	}
	if (InputInterface::inputGetActive("Decrease Screaming Range")) {
		EntityComponents::ComponentScream::MAX_SCREAM_DIST -= 4;
	}
	if (InputInterface::inputGetActive("Spawn Insects")) {
		spawnInsects(100, 0.02f);
	}


	for (uint16_t x = 0; x < gameLevel->hearingGrid.size(); x++) {
		for (uint16_t y = 0; y < gameLevel->hearingGrid[x].size(); y++) {
			gameLevel->hearingGrid[x][y].clear();
		}
	}

	for (uint32_t i = 0; i < gameLevel->insects.size(); i++) {
		Entity& entityCur = EntityManager::entitiesVector[gameLevel->insects[i]];

		auto* entityPositionComponent = entityCur.entityComponentGet<EntityComponents::ComponentPosition>();

		sf::Vector2i gridCoordinates = gameLevel->coordinateRealToHearing(entityPositionComponent->x, entityPositionComponent->y);

		gameLevel->hearingGrid[gridCoordinates.x][gridCoordinates.y].push_back(gameLevel->insects[i]);
	}

	gameLevel->screamConnections.clear();
	gameLevel->acceptedScreamConnections.clear();

	LevelUpdater::levelsUpdate();
}
void GameStatePlay::gameStateFirstStart() {
	ObjectGrid::gridInitialize(CellDimensions(sf::Vector2i(4, 4)), 320, 180);

	constexpr uint32_t TOTAL_INSECTS = 100;

	spawnInsects(TOTAL_INSECTS, 0.02f);
}
 
void GameStatePlay::spawnInsects(uint32_t insectCount, float ratioOfScouts) {

	GameLevel* gameLevel = GameLevelGrid::levelGet(0, 0, 0);

	for (uint16_t i = 0; i < insectCount * (1.f - ratioOfScouts); i++) {
		EntityId entityId = EntityManager::entityCreate(0, 0, 0, "Insect", EntityUpdateType::Frame);
		gameLevel->insects.push_back(entityId);
	}
	for (uint16_t i = 0; i < insectCount * ratioOfScouts; i++) {
		EntityId entityId = EntityManager::entityCreate(0, 0, 0, "Insect Scout", EntityUpdateType::Frame);
		gameLevel->insects.push_back(entityId);
	}
}

void GameStatePause::gameStateUpdate() {
}



