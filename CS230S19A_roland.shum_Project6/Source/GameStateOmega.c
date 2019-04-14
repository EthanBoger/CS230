//------------------------------------------------------------------------------
//
// File Name:	GameStateOmega.c
// Author(s):	Roland Shum
// Project:		MyGame
// Course:		CS230S19
//
// Copyright © 2019 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include <AEEngine.h>
#include "GameStateManager.h"
#include "GameStateOmega.h"
#include "Trace.h"
#include "GameStateManager.h"
#include "GameObjectManager.h"

#include "Vector2D.h"
#include "GameObjectFactory.h"

#include "ScoreSystem.h"

//------------------------------------------------------------------------------
// Private Consts:
//------------------------------------------------------------------------------
static const int cAsteroidSpawnInitial = 8;
static const int cAsteroidSpawnMaximum = 20;
//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------
static int asteroidSpawnCount;
//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------
static void GameStateAsteroidsCreateHudElements(void);
static void GameStateAsteroidsSpawnAsteroidWave(void);
static void GameStateAsteroidsSpawnAsteroid(void);
//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Load the resources associated with the Omega game state.
void GameStateOmegaLoad()
{
	ScoreSystemResetScores();
}

// Initialize the memory associated with the Omega game state.
void GameStateOmegaInit()
{
	// Add one type of each object into the archetype
	GameObjectManagerAdd(GameObjectFactoryCreate(cGameObjectTypeArena));

	GameObjectManagerAddArchetype(GameObjectFactoryCreate(cGameObjectTypeAsteroid));
	GameObjectManagerAddArchetype(GameObjectFactoryCreate(cGameObjectTypeBullet));
	GameObjectManagerAddArchetype(GameObjectFactoryCreate(cGameObjectTypeHudText));
	GameStateAsteroidsCreateHudElements();
	GameObjectPtr spaceship = (GameObjectFactoryCreate(cGameObjectTypeSpaceship));
	GameObjectManagerAdd(spaceship);
	//Vector2D translation = { 0, -150 };
	Vector2D translation = { 300, -150 };
	TransformSetTranslation(GameObjectGetTransform(spaceship), &translation);

	asteroidSpawnCount = cAsteroidSpawnInitial;

	// Spawn waves
	ScoreSystemRestart();
	GameStateAsteroidsSpawnAsteroidWave();

	AEGfxSetBackgroundColor(0, 0, 0);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
}

// Update the Omega game state.
// Params:
//	 dt = Change in time (in seconds) since the last game loop.
void GameStateOmegaUpdate(float dt)
{
	/* Tell the compiler that the 'dt' variable is unused. */
	UNREFERENCED_PARAMETER(dt);

	if (AEInputCheckTriggered('1'))
	{
		GameStateManagerSetNextState(GsLevel1);
	}
	else if (AEInputCheckTriggered('2'))
	{
		GameStateManagerSetNextState(GsLevel2);
	}
	else if (AEInputCheckTriggered('3'))
	{
		GameStateManagerSetNextState(GsAsteroids);
	}
	else if (AEInputCheckTriggered('4'))
	{
		GameStateManagerSetNextState(GsRestart);
	}
	if (GameObjectManagerGetObjectByName("Asteroid") == NULL)
	{
		GameStateAsteroidsSpawnAsteroidWave();
	}
}

// Free any memory associated with the Omega game state.
void GameStateOmegaShutdown()
{
	GameObjectManagerShutdown();
}

// Unload the resources associated with the Omega game state.
void GameStateOmegaUnload()
{
	GameObjectFactoryUnloadResources();
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

static void GameStateAsteroidsCreateHudElements(void)
{
	Vector2D positions[3] = { { -150, 50 },{ -50,0 },{ -30,-50 } };
	ScoreSystemCreateHudElements(positions);
}


static void GameStateAsteroidsSpawnAsteroidWave(void)
{
	ScoreSystemIncreaseWaveCount();

	for (int i = 0; i < asteroidSpawnCount; i++)
	{
		GameStateAsteroidsSpawnAsteroid();
	}
	asteroidSpawnCount++;

	// Limit it.
	if (asteroidSpawnCount > cAsteroidSpawnMaximum)
	{
		asteroidSpawnCount = cAsteroidSpawnMaximum;
	}
}

static void GameStateAsteroidsSpawnAsteroid(void)
{
	GameObjectPtr obj = GameObjectManagerGetArchetype("Asteroid");
	GameObjectPtr clone = GameObjectClone(obj);
	GameObjectManagerAdd(clone);
}