#include "ACECS.hpp"

void Engine::inputsRegister() {
	InputInterface::inputRegister("Pause", KeySet{ KeyEvent("Escape", Pressed) });
}

// game states are registered here
void Engine::panelsRegister() {
	using namespace PanelManager;
	panelAdd("Game View",
		PanelRect(0, 0, 1280, 720), // screen coordinates
		PanelRect(0, 0, 1280, 720), // world coordinates
		sf::Color::Black,
		PANEL_DRAW_FUNCTION{
			GameLevel * gameLevel = static_cast<GameLevel*>(WorldGrid::levelGet(0, 0, 0));
			
			for (EntityId i = 0; i < gameLevel->entities.size(); i++) {
				Entity& entity = EntityManager::entitiesVector[i];

				auto* entityPositionComponent = entity.entityComponentGet<EntityComponents::ComponentPosition>();

				if (entityPositionComponent) {
					sf::CircleShape circle(2);

					circle.setPosition(entityPositionComponent->x, entityPositionComponent->y);

					panel.objectDraw(circle);
				}
			}
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

				for (uint16_t i = 0; i < 1000; i++) {
					EntityId entityId = EntityManager::entityCreate(0, 0, 0, EntityUpdateType::Frame);
					ComponentTemplateManager::componentTemplateApply("Insect", entityId);
					
					Entity& entityInstance = EntityManager::entitiesVector[entityId];

					auto* entityPositionComponent = entityInstance.entityComponentGet<EntityComponents::ComponentPosition>();
					entityPositionComponent->x = 640;
					entityPositionComponent->y = 360;

					entityInstance.entityComponentGet<EntityComponents::ComponentRotation>()->rotation = RNGf::getFullRange(float(M_PI));
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
