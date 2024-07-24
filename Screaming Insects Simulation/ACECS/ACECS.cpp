#include "ACECS.hpp"

void Engine::inputsRegister() {
	InputInterface::inputRegister("Enter Play", KeySet{ KeyEvent("Escape", Pressed) });
	InputInterface::inputRegister("Toggle Scream View", KeySet{ KeyEvent("Num1", Pressed) });
	InputInterface::inputRegister("Toggle Hearing View", KeySet{ KeyEvent("Num2", Pressed) });
	InputInterface::inputRegister("Spawn Home", KeySet{ KeyEvent("LControl", Held), KeyEvent("Mouse Left", Pressed) }, InputKeyLogic::And);
	InputInterface::inputRegister("Spawn Food", KeySet{ KeyEvent("LControl", Held), KeyEvent("Mouse Right", Pressed) }, InputKeyLogic::And);
	InputInterface::inputRegister("Remove Target", KeySet{ KeyEvent("LControl", Held), KeyEvent("Mouse Middle", Pressed) }, InputKeyLogic::And);

	InputInterface::inputRegister("Move Panel Right", KeySet{ KeyEvent("D", Held), KeyEvent("Right", Held) });
	InputInterface::inputRegister("Move Panel Up", KeySet{ KeyEvent("W", Held), KeyEvent("Up", Held) });
	InputInterface::inputRegister("Move Panel Left", KeySet{ KeyEvent("A", Held), KeyEvent("Left", Held) });
	InputInterface::inputRegister("Move Panel Down", KeySet{ KeyEvent("S", Held), KeyEvent("Down", Held) });
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
	panelAdd("Scream View",
		PanelRect(0, 0, 1280, 720), // screen coordinates
		PanelRect(0, 0, 1280, 720), // world coordinates
		sf::Color::Transparent,
		PANEL_DRAW_FUNCTION{
			GameLevel* gameLevel = static_cast<GameLevel*>(WorldGrid::levelGet(0, 0, 0));

			sf::VertexArray vertexArray(sf::Lines, gameLevel->screamConnections.size() * 2);

			for (uint32_t i = 0; i < gameLevel->screamConnections.size(); i++) {

				ScreamConnection connectionCur = gameLevel->screamConnections[i];

				uint32_t lineInd = i * 2;

				vertexArray[lineInd + 0] = std::get<0>(connectionCur);
				vertexArray[lineInd + 1] = std::get<1>(connectionCur);

				sf::Color homeColor = sf::Color(255, 255, 0, 255 / 8);
				sf::Color foodColor = sf::Color(0, 255, 255, 255 / 8);

				sf::Color screamColor = std::get<2>(connectionCur) == TargetType::Home ? homeColor : foodColor;

				vertexArray[lineInd + 0].color = screamColor;
				vertexArray[lineInd + 1].color = screamColor;
			}

			panel.objectDraw(vertexArray);
		}
	);
	panelAdd("Hearing View",
		PanelRect(0, 0, 1280, 720), // screen coordinates
		PanelRect(0, 0, 1280, 720), // world coordinates
		sf::Color::Transparent,
		PANEL_DRAW_FUNCTION{
			GameLevel * gameLevel = static_cast<GameLevel*>(WorldGrid::levelGet(0, 0, 0));

			sf::VertexArray vertexArray(sf::Lines, gameLevel->acceptedScreamConnections.size() * 2);

			for (uint32_t i = 0; i < gameLevel->acceptedScreamConnections.size(); i++) {

				ScreamConnection connectionCur = gameLevel->acceptedScreamConnections[i];

				uint32_t lineInd = i * 2;

				vertexArray[lineInd + 0] = std::get<0>(connectionCur);
				vertexArray[lineInd + 1] = std::get<1>(connectionCur);

				sf::Color homeColor = sf::Color(255, 255, 0, 255 / 4);
				sf::Color foodColor = sf::Color(0, 255, 255, 255 / 4);

				sf::Color screamColor = std::get<2>(connectionCur) == TargetType::Home ? homeColor : foodColor;

				vertexArray[lineInd + 0].color = screamColor;
				vertexArray[lineInd + 1].color = screamColor;
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
			GameStateTransition("Scream State", // name of the state to transition to
				/// transition inputs
				// vector of inputs that trigger this transition
				// note the commas after an input name, without commas every name will become a single string
				{
					"Toggle Scream View",
				}
			),
			GameStateTransition("Hearing State", // name of the state to transition to
				/// transition inputs
				// vector of inputs that trigger this transition
				// note the commas after an input name, without commas every name will become a single string
				{
					"Toggle Hearing View",
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

			float zoomAmount = 1.f - (InputInterface::mouseScrollAmountGet() / 32.f);

			PanelManager::panelGet("Game View")->zoomView(zoomAmount);
			PanelManager::panelGet("Scream View")->zoomView(zoomAmount);
			PanelManager::panelGet("Hearing View")->zoomView(zoomAmount);

			float panelMoveAmountX = (InputInterface::inputGetActive("Move Panel Right") - InputInterface::inputGetActive("Move Panel Left")) * 4.f;
			float panelMoveAmountY = (InputInterface::inputGetActive("Move Panel Down") - InputInterface::inputGetActive("Move Panel Up")) * 4.f;

			PanelManager::panelGet("Game View")->viewRect.left += panelMoveAmountX;
			PanelManager::panelGet("Game View")->viewRect.top += panelMoveAmountY;

			PanelManager::panelGet("Scream View")->viewRect.left += panelMoveAmountX;
			PanelManager::panelGet("Scream View")->viewRect.top += panelMoveAmountY;

			PanelManager::panelGet("Hearing View")->viewRect.left += panelMoveAmountX;
			PanelManager::panelGet("Hearing View")->viewRect.top += panelMoveAmountY;

			GameLevel* gameLevel = static_cast<GameLevel*>(WorldGrid::levelGet(0, 0, 0));

			if (gameLevel->firstRun) {

				gameLevel->firstRun = false;

				ObjectGrid::gridInitialize(CellDimensions(sf::Vector2i(4, 4)), 320, 180);

				constexpr uint32_t TOTAL_INSECTS = 5000;
				constexpr float RATIO_OF_SCOUTS = 0.05f;

				for (uint16_t i = 0; i < TOTAL_INSECTS * (1.f - RATIO_OF_SCOUTS); i++) {
					EntityId entityId = EntityManager::entityCreate(0, 0, 0, EntityUpdateType::Frame);
					ComponentTemplateManager::componentTemplateApply("Insect", entityId);
					
					Entity& entityInstance = EntityManager::entitiesVector[entityId];

					auto* entityPositionComponent = entityInstance.entityComponentGet<EntityComponents::ComponentPosition>();
					entityPositionComponent->x = RNGf::getRange(16, 1280-16);
					entityPositionComponent->y = RNGf::getRange(16, 720-16);

					entityInstance.entityComponentGet<EntityComponents::ComponentRotation>()->rotation = RNGf::getFullRange(Mathf::PI);

					entityInstance.entityComponentGet<EntityComponents::ComponentMoveByRotation>()->moveSpeed = RNGf::getRange(50.f, 110.f);

					gameLevel->insects.push_back(entityId);
				}
				for (uint16_t i = 0; i < TOTAL_INSECTS * RATIO_OF_SCOUTS; i++) {
					EntityId entityId = EntityManager::entityCreate(0, 0, 0, EntityUpdateType::Frame);
					ComponentTemplateManager::componentTemplateApply("Insect Scout", entityId);

					Entity& entityInstance = EntityManager::entitiesVector[entityId];

					auto* entityPositionComponent = entityInstance.entityComponentGet<EntityComponents::ComponentPosition>();
					entityPositionComponent->x = RNGf::getRange(16, 1280 - 16);
					entityPositionComponent->y = RNGf::getRange(16, 720 - 16);

					entityInstance.entityComponentGet<EntityComponents::ComponentRotation>()->rotation = RNGf::getFullRange(Mathf::PI);

					entityInstance.entityComponentGet<EntityComponents::ComponentMoveByRotation>()->moveSpeed = RNGf::getRange(50.f, 110.f);
					gameLevel->insects.push_back(entityId);
				}
			}

			if ((!InputInterface::inputGetActive("Spawn Home")) != (!InputInterface::inputGetActive("Spawn Food"))) {
				EntityId entityId = EntityManager::entityCreate(0, 0, 0, EntityUpdateType::Frame);
				ComponentTemplateManager::componentTemplateApply("Target", entityId);

				Entity& entityInstance = EntityManager::entitiesVector[entityId];

				auto* entityPositionComponent = entityInstance.entityComponentGet<EntityComponents::ComponentPosition>();
				entityPositionComponent->x = float(InputInterface::mousePositionGet().x);
				entityPositionComponent->y = float(InputInterface::mousePositionGet().y);

				float populationRadius = 8 * 4;

				auto* gridCellPopulatorComponent = entityInstance.entityComponentGet<EntityComponents::ComponentObjectGridCellPopulator>();
				gridCellPopulatorComponent->popType = InputInterface::inputGetActive("Spawn Home") ? TargetType::Home : TargetType::Food;
				gridCellPopulatorComponent->popRadius = populationRadius;

				auto* gridCellDepopulatorComponent = entityInstance.entityComponentGet<EntityComponents::ComponentObjectGridCellDepopulator>();
				gridCellDepopulatorComponent->popRadius = populationRadius;


				entityInstance.entityComponentGet<EntityComponents::ComponentRotation>()->rotation = RNGf::getFullRange(Mathf::PI);

				//entityInstance.entityComponentGet<EntityComponents::ComponentMoveByRotation>()->moveSpeed = RNGf::getRange(30.f, 45.f);

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

			for (uint16_t x = 0; x < gameLevel->hearingGrid.size(); x++) {
				for (uint16_t y = 0; y < gameLevel->hearingGrid[x].size(); y++) {
					gameLevel->hearingGrid[x][y].clear();
				}
			}

			for (EntityId i = 0; i < gameLevel->insects.size(); i++) {
				Entity& entityCur = EntityManager::entitiesVector[gameLevel->insects[i]];

				auto* entityPositionComponent = entityCur.entityComponentGet<EntityComponents::ComponentPosition>();

				sf::Vector2i gridCoordinates = gameLevel->coordinateRealToHearing(entityPositionComponent->x, entityPositionComponent->y);

				gameLevel->hearingGrid[gridCoordinates.x][gridCoordinates.y].push_back(gameLevel->insects[i]);
			}

			gameLevel->screamConnections.clear();
			gameLevel->acceptedScreamConnections.clear();

			LevelUpdater::levelsUpdate();
		}
	);
	GameStateHandler::gameStateAdd("Scream State",
		/// transitions
		// vector of GameStateTransitions, and their inputs
		{
			GameStateTransition("Play", // name of the state to transition to
				/// transition inputs
				// vector of inputs that trigger this transition
				// note the commas after an input name, without commas every name will become a single string
				{
					"Toggle Scream View",
					"Enter Play",
				}
			),
			GameStateTransition("Hearing State", // name of the state to transition to
				/// transition inputs
				// vector of inputs that trigger this transition
				// note the commas after an input name, without commas every name will become a single string
				{
					"Toggle Hearing View",
				}
			)
		},
		/// panels
		// the panels belonging to this GameState,
		// note the commas after every panel name, without commas every name will become a single string
		{
			"Scream View",
		},

		/// update function
		// update function for this GameState, called every frame
		GAME_STATE_FUNCTION {

			float zoomAmount = 1.f - (InputInterface::mouseScrollAmountGet() / 32.f);

			PanelManager::panelGet("Game View")->zoomView(zoomAmount);
			PanelManager::panelGet("Scream View")->zoomView(zoomAmount);
			PanelManager::panelGet("Hearing View")->zoomView(zoomAmount);

			float panelMoveAmountX = (InputInterface::inputGetActive("Move Panel Right") - InputInterface::inputGetActive("Move Panel Left")) * 4.f;
			float panelMoveAmountY = (InputInterface::inputGetActive("Move Panel Down") - InputInterface::inputGetActive("Move Panel Up")) * 4.f;

			PanelManager::panelGet("Game View")->viewRect.left += panelMoveAmountX;
			PanelManager::panelGet("Game View")->viewRect.top += panelMoveAmountY;

			PanelManager::panelGet("Scream View")->viewRect.left += panelMoveAmountX;
			PanelManager::panelGet("Scream View")->viewRect.top += panelMoveAmountY;

			PanelManager::panelGet("Hearing View")->viewRect.left += panelMoveAmountX;
			PanelManager::panelGet("Hearing View")->viewRect.top += panelMoveAmountY;

			GameLevel* gameLevel = static_cast<GameLevel*>(WorldGrid::levelGet(0, 0, 0));

			for (uint16_t x = 0; x < gameLevel->hearingGrid.size(); x++) {
				for (uint16_t y = 0; y < gameLevel->hearingGrid[x].size(); y++) {
					gameLevel->hearingGrid[x][y].clear();
				}
			}

			for (EntityId i = 0; i < gameLevel->insects.size(); i++) {
				Entity& entityCur = EntityManager::entitiesVector[gameLevel->insects[i]];

				auto* entityPositionComponent = entityCur.entityComponentGet<EntityComponents::ComponentPosition>();

				sf::Vector2i gridCoordinates = gameLevel->coordinateRealToHearing(entityPositionComponent->x, entityPositionComponent->y);

				gameLevel->hearingGrid[gridCoordinates.x][gridCoordinates.y].push_back(gameLevel->insects[i]);
			}

			gameLevel->screamConnections.clear();
			gameLevel->acceptedScreamConnections.clear();
			
			LevelUpdater::levelsUpdate();
		}
		);

	GameStateHandler::gameStateAdd("Hearing State",
		/// transitions
		// vector of GameStateTransitions, and their inputs
		{
			GameStateTransition("Play", // name of the state to transition to
				/// transition inputs
				// vector of inputs that trigger this transition
				// note the commas after an input name, without commas every name will become a single string
				{
					"Toggle Hearing View",
					"Enter Play",
				}
			),
			GameStateTransition("Scream State", // name of the state to transition to
				/// transition inputs
				// vector of inputs that trigger this transition
				// note the commas after an input name, without commas every name will become a single string
				{
					"Toggle Scream View",
				}
			)
		},
		/// panels
		// the panels belonging to this GameState,
		// note the commas after every panel name, without commas every name will become a single string
		{
			//"Game View",
			"Hearing View",
		},

		/// update function
		// update function for this GameState, called every frame
		GAME_STATE_FUNCTION{

			float zoomAmount = 1.f - (InputInterface::mouseScrollAmountGet() / 32.f);

			PanelManager::panelGet("Game View")->zoomView(zoomAmount);
			PanelManager::panelGet("Scream View")->zoomView(zoomAmount);
			PanelManager::panelGet("Hearing View")->zoomView(zoomAmount);

			float panelMoveAmountX = (InputInterface::inputGetActive("Move Panel Right") - InputInterface::inputGetActive("Move Panel Left")) * 4.f;
			float panelMoveAmountY = (InputInterface::inputGetActive("Move Panel Down") - InputInterface::inputGetActive("Move Panel Up")) * 4.f;

			PanelManager::panelGet("Game View")->viewRect.left += panelMoveAmountX;
			PanelManager::panelGet("Game View")->viewRect.top += panelMoveAmountY;

			PanelManager::panelGet("Scream View")->viewRect.left += panelMoveAmountX;
			PanelManager::panelGet("Scream View")->viewRect.top += panelMoveAmountY;

			PanelManager::panelGet("Hearing View")->viewRect.left += panelMoveAmountX;
			PanelManager::panelGet("Hearing View")->viewRect.top += panelMoveAmountY;

			GameLevel * gameLevel = static_cast<GameLevel*>(WorldGrid::levelGet(0, 0, 0));

			for (uint16_t x = 0; x < gameLevel->hearingGrid.size(); x++) {
				for (uint16_t y = 0; y < gameLevel->hearingGrid[x].size(); y++) {
					gameLevel->hearingGrid[x][y].clear();
				}
			}

			for (EntityId i = 0; i < gameLevel->insects.size(); i++) {
				Entity& entityCur = EntityManager::entitiesVector[gameLevel->insects[i]];

				auto* entityPositionComponent = entityCur.entityComponentGet<EntityComponents::ComponentPosition>();

				sf::Vector2i gridCoordinates = gameLevel->coordinateRealToHearing(entityPositionComponent->x, entityPositionComponent->y);

				gameLevel->hearingGrid[gridCoordinates.x][gridCoordinates.y].push_back(gameLevel->insects[i]);
			}

			gameLevel->screamConnections.clear();
			gameLevel->acceptedScreamConnections.clear();

			LevelUpdater::levelsUpdate();
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
