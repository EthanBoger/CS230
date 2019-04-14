//------------------------------------------------------------------------------
//
// File Name:	GameStateAsteroids.c
// Author(s):	Roland Shum
// Project:		MyGame
// Course:		CS230S19
//
// Copyright © 2019 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include <AEEngine.h>
#include "GameStateAsteroids.h"
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

// Initialize the ...
void GameStateAsteroidsInit()
{
	// Add one type of each object into the archetype
	GameObjectManagerAdd(GameObjectFactoryCreate(cGameObjectTypeSpaceship));
	GameObjectManagerAddArchetype(GameObjectFactoryCreate(cGameObjectTypeAsteroid));
	GameObjectManagerAddArchetype(GameObjectFactoryCreate(cGameObjectTypeBullet));
	GameObjectManagerAddArchetype(GameObjectFactoryCreate(cGameObjectTypeHudText));
	GameStateAsteroidsCreateHudElements();

	asteroidSpawnCount = cAsteroidSpawnInitial;

	// Spawn waves
	ScoreSystemRestart();
	GameStateAsteroidsSpawnAsteroidWave();

	AEGfxSetBackgroundColor(0, 0, 0);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
}

void GameStateAsteroidsLoad()
{
	ScoreSystemResetScores();
}


// Update the ...
// Params:
//	 dt = Change in time (in seconds) since the last game loop.
void GameStateAsteroidsUpdate(float dt)
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
    GameStateManagerSetNextState(GsRestart);
  }
  else if (AEInputCheckTriggered('4'))
  {
	  GameStateManagerSetNextState(GsOmega);
  }
  if (GameObjectManagerGetObjectByName("Asteroid") == NULL)
  {
	  GameStateAsteroidsSpawnAsteroidWave();
  }
}

// Shutdown the ...
void GameStateAsteroidsShutdown()
{
	GameObjectManagerShutdown();
}

void GameStateAsteroidsUnload()
{
	GameObjectFactoryUnloadResources();
}


//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------


static void GameStateAsteroidsCreateHudElements(void)
{
	Vector2D positions[3] = { { -375, 270}, {-275,230}, {230,270} };
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