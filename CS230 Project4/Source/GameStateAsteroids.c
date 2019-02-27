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
#include "BehaviorSpaceship.h"
#include "BehaviorBullet.h"
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
static AEGfxVertexList*	pMeshSpaceship = NULL;
static AEGfxVertexList*	pMeshBullet = NULL;

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------
static void GameStateAsteroidsCreateMeshes(void);
static void GameStateAsteroidsFreeMeshes(void);
static void GameStateAsteroidsCreateSpaceship(void);
static void GameStateAsteroidsCreateBulletArchetype(void);

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Initialize the ...
void GameStateAsteroidsInit()
{
  GameStateAsteroidsCreateSpaceship();
  GameStateAsteroidsCreateBulletArchetype();

  AEGfxSetBackgroundColor(0, 0, 0);
  AEGfxSetBlendMode(AE_GFX_BM_BLEND);
}

void GameStateAsteroidsLoad()
{
  GameStateAsteroidsCreateMeshes();
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
}

// Shutdown the ...
void GameStateAsteroidsShutdown()
{
  GameObjectManagerShutdown();
}

void GameStateAsteroidsUnload()
{
  GameStateAsteroidsFreeMeshes();
}


//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

static void GameStateAsteroidsCreateMeshes(void)
{
  // Create triangle spaceship
  AEGfxMeshStart();
  AEGfxTriAdd(
    -0.5f, -0.5f, 0xFFFF0000, 0.0f, 0.0f,
    0.5f, 0.0f, 0xFFFFFF00, 0.0f, 0.0f,
    -0.5f, 0.5f, 0xFFFF0000, 0.0f, 0.0f);
  pMeshSpaceship = AEGfxMeshEnd();
  AE_ASSERT_MESG(pMeshSpaceship, "Failed to create spaceship mesh!");

  // Create bullet mesh
  AEGfxMeshStart();
  AEGfxTriAdd(
    -0.5f, -0.5f, 0xFF808080, 0.0f, 0.0f,
    0.5f, 0.0f, 0xFFFF0080, 0.0f, 0.0f,
    -0.5f, 0.5f, 0xFF808080, 0.0f, 0.0f);
  pMeshBullet = AEGfxMeshEnd();
  AE_ASSERT_MESG(pMeshBullet, "Failed to create bullet mesh!");

}

static void GameStateAsteroidsFreeMeshes(void)
{
  AEGfxMeshFree(pMeshSpaceship);
  pMeshSpaceship = NULL;

  AEGfxMeshFree(pMeshBullet);
  pMeshBullet = NULL;
}

static void GameStateAsteroidsCreateSpaceship(void)
{
  GameObjectPtr newObj = GameObjectCreate("Spaceship");
  TransformPtr trans = TransformCreate(0, 0);

  /* Error checking. */
  if (trans == NULL)
    return;

  TransformSetRotation(trans, 0);
  /* Set the scale by making a vector 2d of 100,100 */
  Vector2D vector_scale = { 50, 40 };
  TransformSetScale(trans, &vector_scale);

  SpritePtr sprite = SpriteCreate("Spaceship Sprite");

  /* Error checking */
  if (sprite == NULL)
    return;

  SpriteSetMesh(sprite, pMeshSpaceship);

  PhysicsPtr physics = PhysicsCreate();
  BehaviorPtr behavior = BehaviorSpaceshipCreate();

  /* Error checking */
  if (physics == NULL)
    return;

  GameObjectAddPhysics(newObj, physics);
  GameObjectAddTransform(newObj, trans);
  GameObjectAddSprite(newObj, sprite);
  GameObjectAddBehavior(newObj, behavior);

  GameObjectManagerAdd(newObj);
  return;
}

static void GameStateAsteroidsCreateBulletArchetype(void)
{
  GameObjectPtr newObj = GameObjectCreate("Bullet");
  TransformPtr trans = TransformCreate(0, 0);

  /* Error checking. */
  if (trans == NULL)
    return;

  TransformSetRotation(trans, 0);
  /* Set the scale by making a vector 2d of 100,100 */
  Vector2D vector_scale = { 10,10 };
  TransformSetScale(trans, &vector_scale);

  SpritePtr sprite = SpriteCreate("Bullet Sprite");

  /* Error checking */
  if (sprite == NULL)
    return;

  SpriteSetMesh(sprite, pMeshSpaceship);

  PhysicsPtr physics = PhysicsCreate();
  BehaviorPtr behavior = BehaviorBulletCreate();

  /* Error checking */
  if (physics == NULL)
    return;

  GameObjectAddPhysics(newObj, physics);
  GameObjectAddTransform(newObj, trans);
  GameObjectAddSprite(newObj, sprite);
  GameObjectAddBehavior(newObj, behavior);

  GameObjectManagerAddArchetype(newObj);

  return;
}