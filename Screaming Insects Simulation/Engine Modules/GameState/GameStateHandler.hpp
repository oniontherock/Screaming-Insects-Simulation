#ifndef __GAME_STATE_HANDLER_H__
#define __GAME_STATE_HANDLER_H__

#include "../Graphics/Panel.hpp"
#include "../Input/InputInterface.hpp"
#include <cstdint>
#include <unordered_map>


enum GameStateTypes : uint16_t;

typedef GameStateTypes GameStateType;

struct GameStateTransition {

	GameStateTransition(GameStateType _toStateName, std::vector<InputName> _transitionInputs);

	// the name of the state that this transition leads to
	GameStateType toStateName;

	// vector of inputs that trigger a transition
	std::vector<InputName> linkedInputs;
};

struct GameState {

	GameState(std::vector<GameStateTransition> _transitions, std::vector<PanelName> _panelNames);
	virtual ~GameState() {};

	// the name of this GameState
	GameStateType id;

	// vector of different GameStateTransitions linked to this GameStates
	std::vector<GameStateTransition> transitions;

	// vector of different PanelNames belonging to this GameState
	std::vector<PanelName> panelNames;

	// this GameState's update function
	virtual void gameStateUpdate() = 0;
};

typedef std::unique_ptr<GameState> GameStateUniquePtr;

class GameStateHandler {
	static std::unordered_map<GameStateType, GameStateUniquePtr> gameStates;
	static GameStateType gameStateNameCur;

	static void gameStateUpdate();
	static const void gameStateRun();
	// checks if gameStateCurName is valid, and if it is, returns true, otherwise, prints an error and returns false
	static bool gameStateNameCurCheckValid();

public:
	static bool gameStateExists(GameStateType stateName);
	static void gameStateProcess();
	static void gameStateTerminate(GameStateType name);
	static void gameStateAdd(GameStateUniquePtr gameStateInstance);
	// does some error checking on every state, should be called after adding new game states
	static void gameStatesAddedStatesFinalize();
	static void gameStateForceSet(GameStateType gameStateName);
	static void gameStatesAllTerminate();
	static const std::vector<PanelName>& gameStateGetPanels();
};

#endif
