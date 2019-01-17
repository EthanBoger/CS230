//------------------------------------------------------------------------------
//
// File Name:	GameStateTable.c
// Author(s):	Doug Schilling (dschilling)
// Project:		MyGame
// Course:		CS230S19
//
// Copyright © 2019 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "GameStateManager.h"
#include "GameStateTable.h"
#include "Trace.h"

/* Include all game state headers here. */
#include "GameStateLevel1.h"
#include "GameStateLevel2.h"

//------------------------------------------------------------------------------
// Private Consts:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

typedef void(*GameStateVoidFunctionPtr)();
typedef void(*GameStateDtFunctionPtr)(float dt);

typedef struct
{
	const char *				gameStateName;		// The name of the game state as a zero-terminated string.
	GameStateVoidFunctionPtr	gameStateLoad;		/* Pointer to a game state load function. */
	GameStateVoidFunctionPtr	gameStateInit;		/* Pointer to a game state initialization function. */
	GameStateDtFunctionPtr		gameStateUpdate;	/* Pointer to a game state update function. */
	GameStateVoidFunctionPtr	gameStateShutdown;	/* Pointer to a game state shutdown function. */
	GameStateVoidFunctionPtr	gameStateUnload;	/* Pointer to a game state unload function. */

} GameStateTableEntry;

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------

static const GameStateTableEntry GameStateTab[GsNum] =
{
	// Level1 Game State - This is here as a placeholder only.
	{ "Level1",	GameStateLevel1Load, GameStateLevel1Init, GameStateLevel1Update, GameStateLevel1Shutdown, GameStateLevel1Unload },
	{ "Level2",	GameStateLevel2Load, GameStateLevel2Init, GameStateLevel2Update, GameStateLevel2Shutdown, GameStateLevel2Unload }

	// TODO: Add new game states here.
};

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Determine if the game state is valid.
bool GameStateIsValid(GameStates gameState)
{
	return ((0 <= gameState) && (gameState < GsNum));
}

// Determine if the game state is a "special" game state.
bool GameStateIsSpecial(GameStates gameState)
{
	return ((gameState == GsRestart) || (gameState == GsQuit));
}

// Execute the Load function for the current game state.
void GameStateExecuteLoad(GameStates gameState)
{
	// First validate the game state and the function pointer.
	if (GameStateIsValid(gameState) &&
		((*GameStateTab[gameState].gameStateLoad) != NULL))
	{
		// Call TraceMessage, passing the format string "%s: Load" and the name of the game state.
		TraceMessage("%s: Load", GameStateTab[gameState].gameStateName);

		// Execute the Init function.
		(*GameStateTab[gameState].gameStateLoad)();
	}
}

// Execute the Init function for the current game state.
void GameStateExecuteInit(GameStates gameState)
{
	// First validate the game state and the function pointer.
	if (GameStateIsValid(gameState) &&
		((*GameStateTab[gameState].gameStateInit) != NULL))
	{
		// Call TraceMessage, passing the format string "%s: Init" and the name of the game state.
		TraceMessage("%s: Init", GameStateTab[gameState].gameStateName);
		// Execute the Init function.
		(*GameStateTab[gameState].gameStateInit)();
	}
}

// Execute the Update function for the current game state.
void GameStateExecuteUpdate(GameStates gameState, float dt)
{
	// First validate the game state and the function pointer.
	if (GameStateIsValid(gameState) &&
		((*GameStateTab[gameState].gameStateUpdate) != NULL))
	{
		// Call TraceMessage, passing the format string "%s: Update" and the name of the game state.
		TraceMessage("%s: Update", GameStateTab[gameState].gameStateName);
		// Execute the Update function.
		(*GameStateTab[gameState].gameStateUpdate)(dt);
	}
}

// Execute the Shutdown function for the current game state.
void GameStateExecuteShutdown(GameStates gameState)
{
	// First validate the game state and the function pointer.
	if (GameStateIsValid(gameState) &&
		((*GameStateTab[gameState].gameStateShutdown) != NULL))
	{
		// Call TraceMessage, passing the format string "%s: Shutdown" and the name of the game state.
		TraceMessage("%s: Shutdown", GameStateTab[gameState].gameStateName);
		// Execute the Shutdown function.
		(*GameStateTab[gameState].gameStateShutdown)();
	}
}

// Execute the Unload function for the current game state.
void GameStateExecuteUnload(GameStates gameState)
{
	// First validate the game state and the function pointer.
	if (GameStateIsValid(gameState) &&
		((*GameStateTab[gameState].gameStateUnload) != NULL))
	{
		// Call TraceMessage, passing the format string "%s: Unload" and the name of the game state.
		TraceMessage("%s: Unload", GameStateTab[gameState].gameStateName);
		// Execute the Init function.
		(*GameStateTab[gameState].gameStateUnload)();
	}
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

