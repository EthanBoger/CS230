//------------------------------------------------------------------------------
//
// File Name:	Stub.c
// Author(s):	Roland Shum
// Project:		MyGame
// Course:		CS230S19
//
// Copyright © 2019 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Stub.h"
#include "Vector2D.h"

#include "GameObject.h"
#include "BehaviorHudText.h"
#include "GameObjectManager.h"
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
static int asteroidScore = 0;
static int asteroidHighScore = 0;
static int asteroidWaveCount;
//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------
static void GameStateAsteroidsCreateHudElement(const GameObjectPtr archetype, const Vector2D * position, const char * formatString, const int * watchValue);

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Reset the Score system.
// (NOTE: All private variables must be initialized to 0.)
void ScoreSystemResetScores()
{
	asteroidScore = 0;
	asteroidHighScore = 0;
	asteroidWaveCount = 0;
}

// Restart the Score system after updating the high score.
// (HINT: The high score value should be updated before zeroing out the score and wave count variables.
void ScoreSystemRestart()
{
	// Update high score.
	if (asteroidScore > asteroidHighScore)
	{
		asteroidHighScore = asteroidScore;
	}
	asteroidScore = 0;
	asteroidWaveCount = 0;
}

// Increase the game score by score value.
// Params:
//	 scoreValue = The amount to be added to the game score.
void ScoreSystemIncreaseScore(unsigned int scoreValue)
{
	asteroidScore += scoreValue;
}

// Increase the wave count by 1.
void ScoreSystemIncreaseWaveCount()
{
	asteroidWaveCount++;
}

// Create HUD Text objects for "High Score", "Score", and "Wave Count".
// (NOTE: The positions variable must point at an array of three Vector2D structures.)
// Params:
//	 positions = Vector2D array of positions for the HUD Text objects.
void ScoreSystemCreateHudElements(const Vector2D * positions)
{
	GameObjectPtr hud = GameObjectManagerGetArchetype("HUD Text");

	if (hud != NULL)
	{
		GameStateAsteroidsCreateHudElement(hud, &positions[0], "High Score: %d", &asteroidHighScore);

		GameStateAsteroidsCreateHudElement(hud, &positions[1], "Score: %d", &asteroidScore);

		GameStateAsteroidsCreateHudElement(hud, &positions[2], "Wave: %d", &asteroidWaveCount);
	}
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

static void GameStateAsteroidsCreateHudElement(const GameObjectPtr archetype, const Vector2D * position, const char * formatString, const int * watchValue)
{
	GameObjectPtr newObj = GameObjectClone(archetype);

	TransformPtr newTransform = GameObjectGetTransform(newObj);
	TransformSetTranslation(newTransform, position);

	BehaviorHudTextSetDisplay(GameObjectGetBehavior(newObj), formatString, watchValue);

	GameObjectManagerAdd(newObj);
}