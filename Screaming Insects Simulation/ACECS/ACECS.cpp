#include "ACECS.hpp"


void Engine::inputsRegister() {
	InputInterface::inputRegister("Example Input", KeySet{ KeyEvent("Escape", Pressed) });
}

// game states are registered here
void Engine::panelsRegister() {
	using namespace PanelManager;
	panelAdd("Example View",
		PanelRect(0, 0, 1280, 720), // screen coordinates
		PanelRect(0, 0, 640, 360), // world coordinates
		sf::Color::Black,
		PANEL_DRAW_FUNCTION{
		}
	);
}

// game states are registered here
void Engine::gameStateRegister() {

	GameStateHandler::gameStateForceSet("Example State");

	GameStateHandler::gameStateAdd("Example State",
		/// transitions
		// vector of GameStateTransitions, and their inputs
		{
			GameStateTransition("Other Example State", // name of the state to transition to
				/// transition inputs
				// vector of inputs that trigger this transition
				// note the commas after an input name, without commas every name will become a single string
				{
					"Example Input",
				}
			)
		},
		/// panels
		// the panels belonging to this GameState,
		// note the commas after every panel name, without commas every name will become a single string
		{
			"Example View",
		},

		/// update function
		// update function for this GameState, called every frame
		GAME_STATE_FUNCTION{
			LevelUpdater::levelsUpdate();
		}
	);
	GameStateHandler::gameStateAdd("Other Example State",
		/// transitions
		// vector of GameStateTransitions, and their inputs
		{
			GameStateTransition("Example State", // name of the state to transition to
				/// transition inputs
				// vector of inputs that trigger this transition
				// note the commas after an input name, without commas every name will become a single string
				{
					"Example Input",
				}
			)
		},
		/// panels
		// the panels belonging to this GameState,
		// note the commas after every panel name, without commas every name will become a single string
		{
			"Example View",
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
	WorldGrid::levelGridInitialize(5, 5, 5);
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
