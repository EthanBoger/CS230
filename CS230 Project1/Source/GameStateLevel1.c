//------------------------------------------------------------------------------
//
// File Name:	GameStateLevel1.c
// Author(s):	Roland Shum
// Project:		MyGame
// Course:		CS230S19
//
// Copyright © 2019 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "GameStateManager.h"
#include "GameStateLevel1.h"
#include "Trace.h"

//------------------------------------------------------------------------------
// Private Consts:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------
static int numLives = 0;
//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Load the resources associated with the Level1 game state.
void GameStateLevel1Load()
{
	FILE * numLivesfile = 0;
	char * fileName = "Data/Level1_Lives.txt";

	/* Load the value of numLives from "Level1_Lives.txt */
	if (fopen_s(&numLivesfile, fileName, "r") != 0)
	{
		// Error.
		TraceMessage("Error opening %s", fileName);
		return;
	}
	/* Read number and set to num lives. */
	fscanf_s(numLivesfile, "%i", &numLives);

	/* Close file. */
	fclose(numLivesfile);
}

// Initialize the memory associated with the Level1 game state.
void GameStateLevel1Init()
{
}

// Update the Level1 game state.
// Params:
//	 dt = Change in time (in seconds) since the last game loop.
void GameStateLevel1Update(float dt)
{
	/* Tell the compiler that the 'dt' variable is unused. */
	UNREFERENCED_PARAMETER(dt);

	/* Minus numlives and if statement at one go. */
	if (--numLives <= 0)
	{
		// Change state
		GameStateManagerSetNextState(GsLevel2);
	}
}

// Shutdown any memory associated with the Level1 game state.
void GameStateLevel1Shutdown()
{
}

// Unload the resources associated with the Level1 game state.
void GameStateLevel1Unload()
{
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

