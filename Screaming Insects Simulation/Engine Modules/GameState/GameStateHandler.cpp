#include "GameStateHandler.hpp"
#include "../Auxiliary/ConsoleHandler.hpp"
#include "../Graphics/PanelManager.hpp"

std::unordered_map<GameStateType, GameStateUniquePtr> GameStateHandler::gameStates = std::unordered_map<GameStateType, GameStateUniquePtr>();
GameStateType GameStateHandler::gameStateNameCur{};

GameStateTransition::GameStateTransition(GameStateType _toStateName, std::vector<InputName> _transitionInputs) :
	toStateName(_toStateName),
	linkedInputs(_transitionInputs)
{}


GameState::GameState(std::vector<GameStateTransition> _transitions, std::vector<PanelName> _panelNames) :
	transitions(_transitions),
	panelNames(_panelNames),
	id() // we initialize id to nothing to avoid a C26495, the actual id is assigned in the derived class constructor
{}

void GameStateHandler::gameStateUpdate() {
	auto& curGameState = gameStates[gameStateNameCur];

	// iterate through the game state's transitions
	for (uint16_t curTransitionInd = 0; curTransitionInd < curGameState->transitions.size(); curTransitionInd++) {
		// get the current transition
		auto& curTransition = curGameState->transitions[curTransitionInd];

		// iterate through every input of the current transition
		for (uint16_t curTransitionInputInd = 0; curTransitionInputInd < curTransition.linkedInputs.size(); curTransitionInputInd++) {
			// check if the current input of the current transition is active, if it is, transition to that state, and end the check
			if (InputInterface::inputGetActive(curTransition.linkedInputs[curTransitionInputInd])) {
				gameStateNameCur = curTransition.toStateName;
				return;
			}
		}
	}
}

const void GameStateHandler::gameStateRun() {
	gameStates[gameStateNameCur]->gameStateUpdate();
}

// checks if gameStateCurName is valid, and if it is, returns true, otherwise, prints an error and returns false
bool GameStateHandler::gameStateNameCurCheckValid() {
	if (!gameStateExists(gameStateNameCur)) {
		ConsoleHandler::consolePrintErr("Unknown game state: \"" + gameStateNameCur + std::string("\""));
	}
	else {
		return true;
	}
	return false;
}

bool GameStateHandler::gameStateExists(GameStateType stateName) {
	return gameStates.count(stateName);
}

void GameStateHandler::gameStateProcess() {
	if (!gameStateNameCurCheckValid()) return;
	gameStateUpdate();
	gameStateRun();
}

void GameStateHandler::gameStateTerminate(GameStateType name) {
	gameStates.erase(name);
}

void GameStateHandler::gameStateAdd(GameStateUniquePtr gameStateInstance) {

	try {
		if (gameStateExists(gameStateInstance->id)) {
			throw "Attempted to add a GameState with a taken id,\n please ensure the correct class was used when adding the GameState,\n and check the GameState constructor to ensure the correct id was assigned";
		}

		for (const auto& panelNameCur : gameStateInstance->panelNames) {
			if (!PanelManager::panelExists(panelNameCur)) {
				throw "Attempted to add a GameState with non-existent panel: \"" + panelNameCur + std::string("\"");
			}
		}


		gameStates.insert(std::move(std::pair<GameStateType, GameStateUniquePtr>(gameStateInstance->id, std::move(gameStateInstance))));
	}
	catch (const char* errorMessage) {
		ConsoleHandler::consolePrintErr(errorMessage);
		return;
	}
}

// does some error checking on every state, should be called after adding new game states
void GameStateHandler::gameStatesAddedStatesFinalize() {
	for (const auto& [stateNameCur, gameStateCur] : gameStates) {
		for (const auto& gameStateTransitionCur : gameStateCur->transitions) {
			if (!gameStateExists(gameStateTransitionCur.toStateName)) {
				ConsoleHandler::consolePrintErr("AAttempted to add a GameState with non-existent transition: \"" + gameStateTransitionCur.toStateName + std::string("\""));
			}
		}
	}
}

void GameStateHandler::gameStateForceSet(GameStateType gameStateName) {
	gameStateNameCur = gameStateName;
}

void GameStateHandler::gameStatesAllTerminate() {
	gameStates.clear();
}

const std::vector<PanelName>& GameStateHandler::gameStateGetPanels() {
		return gameStates[gameStateNameCur]->panelNames;
	}
