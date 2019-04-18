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
#include "../AE/include/AEEngine.h"
#include "GameStateLevel2.h"
#include "Trace.h"
#include "GameObject.h"
#include "Vector2D.h"
//------------------------------------------------------------------------------
// Private Consts:
//------------------------------------------------------------------------------
static const float spaceshipSpeed = 500.0f;
//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------
static AEGfxVertexList *pMeshSpaceship;
static GameObjectPtr planetObj;

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------
GameObjectPtr GameStateLevel2CreateSpaceship(void);
void GameStateLevel2MovementController(GameObjectPtr gameObject);
//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Load the resources associated with the Level2 game state.
void GameStateLevel2Load()
{
  AEGfxMeshStart();
  AEGfxTriAdd(
    -0.5f, -0.5f, 0xFFFF0000, 0.0f, 0.0f,
    0.5f, 0.0f, 0xFFFFFF00, 0.0f, 0.0f,
    -0.5f, 0.5f, 0xFFFF0000, 0.0f, 0.0f);
  pMeshSpaceship = AEGfxMeshEnd();
}

// Initialize the memory associated with the Level2 game state.
void GameStateLevel2Init()
{	
  planetObj = GameStateLevel2CreateSpaceship();
  AEGfxSetBackgroundColor(0, 0, 0);
  AEGfxSetBlendMode(AE_GFX_BM_BLEND);
}

// Update the Level2 game state.
// Params:
//	 dt = Change in time (in seconds) since the last game loop.
void GameStateLevel2Update(float dt)
{
  if (AEInputCheckTriggered('Z'))
  {
	  SpritePtr sprite = planetObj->getSprite();
	  sprite->setAlpha(0.5f);
  }
  if (AEInputCheckTriggered('X'))
  {
	  SpritePtr sprite = planetObj->getSprite();
	  sprite->setAlpha(1.0f);
  }

  /* Update and display. */
  GameStateLevel2MovementController(planetObj);
  planetObj->Update(dt);
  planetObj->Draw();

  if (AEInputCheckTriggered('1'))
  {
    GameStateManagerSetNextState(GsLevel1);
  }
  if (AEInputCheckTriggered('2'))
  {
    GameStateManagerSetNextState(GsRestart);
  }
  if (AEInputCheckTriggered('3'))
  {
    GameStateManagerSetNextState(GsAsteroids);
  }
  else if (AEInputCheckTriggered('4'))
  {
	  GameStateManagerSetNextState(GsOmega);
  }
}

// Shutdown any memory associated with the Level2 game state.
void GameStateLevel2Shutdown()
{
	delete planetObj;
	planetObj = NULL;
}

// Unload the resources associated with the Level2 game state.
void GameStateLevel2Unload()
{
  if (pMeshSpaceship != NULL)
  {
    AEGfxMeshFree(pMeshSpaceship);
  }
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------
GameObjectPtr GameStateLevel2CreateSpaceship(void)
{
  GameObjectPtr newGO = new GameObject("Spaceship");

  TransformPtr transform = new Transform(0, 0);
  Vector2D scale = { 100,100 };
  transform->setScale(&scale);
  transform->setRotation(0);

  SpritePtr sprite = new Sprite("Spaceship Sprite");
  sprite->setMesh(pMeshSpaceship);

  PhysicsPtr physics = new Physics();

  newGO->addPhysics(physics);
  newGO->addSprite(sprite);
  newGO->addTransform(transform);

  return newGO;
}

void GameStateLevel2MovementController(GameObjectPtr GO)
{
  PhysicsPtr physics = GO->getPhysics();
  TransformPtr transform = GO->getTransform();

  /* Error checking. */
  if (!physics && !transform)
  {
    return;
  }

  /* Get mouse position in screen. */
  s32 mx, my;
  AEInputGetCursorPosition(&mx, &my);
  /* Convert screen to world. */
  Vector2D cursor_worldPos;
  AEGfxConvertScreenCoordinatesToWorld((float)mx, (float)my, &cursor_worldPos.x, &cursor_worldPos.y);

  const Vector2D *spaceShip_translation = transform->getTranslation();

  Vector2D space_to_mouse_vector;

  /* find the unit direction from spaceship to mouse. */
  Vector2DSub(&space_to_mouse_vector, &cursor_worldPos, spaceShip_translation);
  Vector2DNormalize(&space_to_mouse_vector, &space_to_mouse_vector);

  /* Set the rotation to that vector we found. */
  float radian = Vector2DToAngleRad(&space_to_mouse_vector);
  transform->setRotation(radian);

  /* Set the velocity by using the direction vector * speed */
  Vector2DScale(&space_to_mouse_vector, &space_to_mouse_vector, spaceshipSpeed);
  physics->setVelocity(&space_to_mouse_vector);
}