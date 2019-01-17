//------------------------------------------------------------------------------
//
// File Name:	GameStateLevel2.c
// Author(s):	Roland Shum
// Project:		MyGame
// Course:		CS230S19
//
// Copyright © 2019 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "GameStateManager.h"
#include "GameStateLevel2.h"
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
static int numHealth = 0;
static int numLives = 0;
//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Load the resources associated with the Level2 game state.
void GameStateLevel2Load()
{
	FILE * numLivesfile;
	char * numLivesFilename = "Data/Level2_Lives.txt";


	/* Load the value of numLives from "Level1_Lives.txt */
	if (fopen_s(&numLivesfile, numLivesFilename, "r") != 0)
	{
		// Error.
		TraceMessage("Error opening %s", numLivesFilename);
		return;
	}
	/* Read number and set to num lives. */
	fscanf_s(numLivesfile, "%i", &numLives);


	/* Close both files. */
	fclose(numLivesfile);
}

// Initialize the memory associated with the Level2 game state.
void GameStateLevel2Init()
{	
	FILE *numHealthfile;
	char * numHealthFilename = "Data/Level2_Health.txt";

	/* Load the value of numLives from "Level1_Lives.txt */
	if (fopen_s(&numHealthfile, numHealthFilename, "r") != 0)
	{
		// Error.
		TraceMessage("Error opening %s", numHealthFilename);
		return;
	}
	/* Read number and set to num lives. */
	fscanf_s(numHealthfile, "%i", &numHealth);

	fclose(numHealthfile);
}

// Update the Level2 game state.
// Params:
//	 dt = Change in time (in seconds) since the last game loop.
void GameStateLevel2Update(float dt)
{
	UNREFERENCED_PARAMETER(dt);
	numHealth--;
	if (numHealth <= 0)
	{
		numLives--;
		if (numLives <= 0)
		{
			GameStateManagerSetNextState(GsQuit);
		}
		else
		{
			GameStateManagerSetNextState(GsRestart);
		}
	}
}

// Shutdown any memory associated with the Level2 game state.
void GameStateLevel2Shutdown()
{
}

// Unload the resources associated with the Level2 game state.
void GameStateLevel2Unload()
{
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

