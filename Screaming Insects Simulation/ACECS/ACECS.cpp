#include "ACECS.hpp"

void Engine::inputsRegister() {
	InputInterface::inputRegister("Pause", KeySet{ KeyEvent("Escape", Pressed) });
	InputInterface::inputRegister("Spawn Home", KeySet{ KeyEvent("LControl", Held), KeyEvent("Mouse Left", Pressed) }, InputKeyLogic::And);
	InputInterface::inputRegister("Spawn Food", KeySet{ KeyEvent("LControl", Held), KeyEvent("Mouse Right", Pressed) }, InputKeyLogic::And);
	InputInterface::inputRegister("Remove Target", KeySet{ KeyEvent("LControl", Held), KeyEvent("Mouse Middle", Pressed) }, InputKeyLogic::And);
}

// game states are registered here
void Engine::panelsRegister() {
	using namespace PanelManager;
	panelAdd("Game View",
		PanelRect(0, 0, 1280, 720), // screen coordinates
		PanelRect(0, 0, 1280, 720), // world coordinates
		sf::Color::Black,
		PANEL_DRAW_FUNCTION{
			GameLevel* gameLevel = static_cast<GameLevel*>(WorldGrid::levelGet(0, 0, 0));
			
			sf::VertexArray vertexArray(sf::Points, gameLevel->entities.size());

			for (EntityId i = 0; i < gameLevel->entities.size(); i++) {
				Entity& entity = EntityManager::entitiesVector[i];

				auto* entityPositionComponent = entity.entityComponentGet<EntityComponents::ComponentPosition>();

				if (entityPositionComponent) {
					if (entity.entityComponentHas<EntityComponents::ComponentObjectGridCellPopulator>()) {
						sf::CircleShape circle(entity.entityComponentGet<EntityComponents::ComponentObjectGridCellPopulator>()->popRadius / 2);

						circle.setOrigin(circle.getRadius(), circle.getRadius());
						circle.setPosition(entityPositionComponent->x, entityPositionComponent->y);
						circle.setFillColor(sf::Color::Blue);

						panel.objectDraw(circle);
					}
					else {
						vertexArray[i] = sf::Vector2f(entityPositionComponent->x, entityPositionComponent->y);
					}
				}
			}
			panel.objectDraw(vertexArray);
		}
	);
}

// game states are registered here
void Engine::gameStateRegister() {

	GameStateHandler::gameStateForceSet("Play");

	GameStateHandler::gameStateAdd("Play",
		/// transitions
		// vector of GameStateTransitions, and their inputs
		{
			GameStateTransition("Pause", // name of the state to transition to
				/// transition inputs
				// vector of inputs that trigger this transition
				// note the commas after an input name, without commas every name will become a single string
				{
					"Pause",
				}
			)
		},
		/// panels
		// the panels belonging to this GameState,
		// note the commas after every panel name, without commas every name will become a single string
		{
			"Game View",
		},

		/// update function
		// update function for this GameState, called every frame
		GAME_STATE_FUNCTION{

			GameLevel* gameLevel = static_cast<GameLevel*>(WorldGrid::levelGet(0, 0, 0));

			if (gameLevel->firstRun) {
				gameLevel->firstRun = false;

				ObjectGrid::gridInitialize(CellDimensions(sf::Vector2i(4, 4)), 320, 180);

				for (uint16_t i = 0; i < 980; i++) {
					EntityId entityId = EntityManager::entityCreate(0, 0, 0, EntityUpdateType::Frame);
					ComponentTemplateManager::componentTemplateApply("Insect", entityId);
					
					Entity& entityInstance = EntityManager::entitiesVector[entityId];

					auto* entityPositionComponent = entityInstance.entityComponentGet<EntityComponents::ComponentPosition>();
					entityPositionComponent->x = RNGf::getRange(16, 1280-16);
					entityPositionComponent->y = RNGf::getRange(16, 720-16);

					entityInstance.entityComponentGet<EntityComponents::ComponentRotation>()->rotation = RNGf::getFullRange(Mathf::PI);

					entityInstance.entityComponentGet<EntityComponents::ComponentMoveByRotation>()->moveSpeed = RNGf::getRange(60.f, 90.f);
				}
				for (uint16_t i = 0; i < 20; i++) {
					EntityId entityId = EntityManager::entityCreate(0, 0, 0, EntityUpdateType::Frame);
					ComponentTemplateManager::componentTemplateApply("Insect Scout", entityId);

					Entity& entityInstance = EntityManager::entitiesVector[entityId];

					auto* entityPositionComponent = entityInstance.entityComponentGet<EntityComponents::ComponentPosition>();
					entityPositionComponent->x = RNGf::getRange(16, 1280 - 16);
					entityPositionComponent->y = RNGf::getRange(16, 720 - 16);

					entityInstance.entityComponentGet<EntityComponents::ComponentRotation>()->rotation = RNGf::getFullRange(Mathf::PI);

					entityInstance.entityComponentGet<EntityComponents::ComponentMoveByRotation>()->moveSpeed = 60.f;
				}
			}

			if ((!InputInterface::inputGetActive("Spawn Home")) != (!InputInterface::inputGetActive("Spawn Food"))) {
				EntityId entityId = EntityManager::entityCreate(0, 0, 0, EntityUpdateType::Frame);
				ComponentTemplateManager::componentTemplateApply("Target", entityId);

				Entity& entityInstance = EntityManager::entitiesVector[entityId];

				auto* entityPositionComponent = entityInstance.entityComponentGet<EntityComponents::ComponentPosition>();
				entityPositionComponent->x = float(InputInterface::mousePositionGet().x);
				entityPositionComponent->y = float(InputInterface::mousePositionGet().y);

				float popRad = 8 * 4;

				auto* gridCellPopulatorComponent = entityInstance.entityComponentGet<EntityComponents::ComponentObjectGridCellPopulator>();
				gridCellPopulatorComponent->popType = InputInterface::inputGetActive("Spawn Home") ? TargetType::Home : TargetType::Food;
				gridCellPopulatorComponent->popRadius = popRad;

				auto* gridCellDepopulatorComponent = entityInstance.entityComponentGet<EntityComponents::ComponentObjectGridCellDepopulator>();
				gridCellDepopulatorComponent->popRadius = popRad;


				entityInstance.entityComponentGet<EntityComponents::ComponentRotation>()->rotation = RNGf::getFullRange(Mathf::PI);

				entityInstance.entityComponentGet<EntityComponents::ComponentMoveByRotation>()->moveSpeed = RNGf::getRange(30.f, 45.f);

				gameLevel->targets.push_back(entityId);
			}
			if (InputInterface::inputGetActive("Remove Target")) {

				for (EntityId i = 0; i < gameLevel->targets.size(); i++) {
					Entity& entityCur = EntityManager::entitiesVector[gameLevel->targets[i]];

					auto* entityPositionComponent = entityCur.entityComponentGet<EntityComponents::ComponentPosition>();

					float axisX = entityPositionComponent->x - float(InputInterface::mousePositionGet().x);
					float axisY = entityPositionComponent->y - float(InputInterface::mousePositionGet().y);

					if ((axisX * axisX) + (axisY * axisY) < 8 * 8) {
						entityCur.entityComponentGet<EntityComponents::ComponentObjectGridCellDepopulator>()->system(entityCur);
						EntityManager::entityTerminate(gameLevel->targets[i]);
					}
				}
			}

			LevelUpdater::levelsUpdate();
		}
	);
	GameStateHandler::gameStateAdd("Pause",
		/// transitions
		// vector of GameStateTransitions, and their inputs
		{
			GameStateTransition("Play", // name of the state to transition to
				/// transition inputs
				// vector of inputs that trigger this transition
				// note the commas after an input name, without commas every name will become a single string
				{
					"Pause",
				}
			)
		},
		/// panels
		// the panels belonging to this GameState,
		// note the commas after every panel name, without commas every name will become a single string
		{
			"Game View",
		},

		/// update function
		// update function for this GameState, called every frame
		GAME_STATE_FUNCTION{
		}
		);

	GameStateHandler::gameStateFinalizeAddedStates();
}

// initialize the ACECS engine by registering all inputs, initializing the ECS module, and registering game states.
// of course, certain modules do not have to be initialized if the user does not want them to be
void Engine::engineInitialize() {
	WorldGrid::levelGridInitialize(1, 1, 1);
	WorldGrid::levelAdd(new GameLevel(0, 0, 0));
	WorldGrid::levelActivate(0, 0, 0);

	inputsRegister();
	ECSRegistry::ECSInitialize();
	panelsRegister();
	gameStateRegister();
}
// updates the engines input
void Engine::engineInputUpdate(sf::RenderWindow& window) {
	InputInterface::inputUpdate();
	InputInterface::eventsProcess(window);
}
// update certain modules of the engine, like the input system, and the game state.
// note that certain modules, like the ECS system, are updated inside the GameStateHandler,
// because you don't want to update the ECS system if the GameState is currently paused, for example.
void Engine::engineUpdate() {
	GameStateHandler::gameStateProcess();
}
void Engine::engineDraw(sf::RenderWindow& renderWindowMain) {

	auto& gameStatePanels = GameStateHandler::gameStateGetPanels();

	for (uint16_t i = 0; i < gameStatePanels.size(); i++) {
		PanelManager::panelGet(gameStatePanels[i])->panelDrawObjects();
		PanelManager::panelGet(gameStatePanels[i])->panelRender(renderWindowMain);
		PanelManager::panelGet(gameStatePanels[i])->panelClear();
	}
}
// terminates certain engine modules, like the ECS or GameStateHandler
void Engine::engineTerminate() {
	ECSRegistry::ECSTerminate();
	GameStateHandler::gameStatesAllTerminate();
	PanelManager::panelManagerTerminate();
}
