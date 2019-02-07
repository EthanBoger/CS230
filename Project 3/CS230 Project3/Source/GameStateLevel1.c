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
#include "Vector2D.h"
#include "../AE/include/AEEngine.h"
#include "SpriteSource.h"
#include "GameObject.h"
#include "Mesh.h"

//------------------------------------------------------------------------------
// Private Consts:
//------------------------------------------------------------------------------
static const float groundHeight = -150.0f;
static const float moveVelocity = 500.0f;
static const float jumpVelocity = 1000.0f;
static const Vector2D gravityNormal = { 0.0f, -1500.0f };
static const Vector2D gravityNone = { 0.0f, 0.0f };

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

static AEGfxVertexList *vertexList;
static AEGfxTexture *texture;
static SpriteSourcePtr spriteSource;
static GameObjectPtr gameObject;


//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------
static GameObjectPtr GameStateLevel1CreatePlanet(void);
void GameStateLevel1MovementController(GameObjectPtr gameObject);
//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Load the resources associated with the Level1 game state.
void GameStateLevel1Load()
{
	FILE * numLivesfile = NULL;
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

  /* Create a mesh with parameters. */
  vertexList = MeshCreateQuad(0.5f, 0.5f, 1.0f, 1.0f, "Mesh1x1");
  if (vertexList == NULL)
    return;

  texture = AEGfxTextureLoad("Assets/PlanetTexture.png");
  if (texture == NULL)
    return;

  spriteSource = SpriteSourceCreate(1, 1, texture);
  if (spriteSource == NULL)
    return;

	/* Close file. */
	fclose(numLivesfile);
}

// Initialize the memory associated with the Level1 game state.
void GameStateLevel1Init()
{
  gameObject = GameStateLevel1CreatePlanet();
  AEGfxSetBackgroundColor(1, 1, 1);
  AEGfxSetBlendMode(AE_GFX_BM_BLEND);
}

// Update the Level1 game state.
// Params:
//	 dt = Change in time (in seconds) since the last game loop.
void GameStateLevel1Update(float dt)
{
  GameStateLevel1MovementController(gameObject);
  GameObjectUpdate(gameObject, dt);
  GameObjectDraw(gameObject);

  /* Restart the scene if the user presses 1. */
  if (AEInputCheckTriggered('1'))
  {
    GameStateManagerSetNextState(GsRestart);
  }
  if (AEInputCheckTriggered('2'))
  {
    GameStateManagerSetNextState(GsLevel2);
  }
}

// Shutdown any memory associated with the Level1 game state.
void GameStateLevel1Shutdown()
{
  GameObjectFree(&gameObject);
}

// Unload the resources associated with the Level1 game state.
void GameStateLevel1Unload()
{
  /* Free sprite source, texture, and texture list. */
  if (spriteSource != NULL)
  {
    SpriteSourceFree(&spriteSource);
  }
  if (texture != NULL)
  {
    AEGfxTextureUnload(texture);
  }
  if (vertexList != NULL)
  {
    AEGfxMeshFree(vertexList);
  }
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

static GameObjectPtr GameStateLevel1CreatePlanet(void)
{
  GameObjectPtr newObj = GameObjectCreate("Planet");
  TransformPtr trans = TransformCreate(0, groundHeight);

  /* Error checking. */
  if (trans == NULL)
    return NULL;

  TransformSetRotation(trans, 0);
  /* Set the scale by making a vector 2d of 100,100 */
  Vector2D obj = { 100,100 };
  TransformSetScale(trans, &obj);

  SpritePtr sprite = SpriteCreate("Planet Sprite");

  /* Error checking */
  if (sprite == NULL)
    return NULL;

  SpriteSetMesh(sprite, vertexList);
  SpriteSetSpriteSource(sprite, spriteSource);


  PhysicsPtr physics = PhysicsCreate();
  /* Error checking */
  if (physics == NULL)
    return NULL;

  GameObjectAddPhysics(newObj, physics);
  GameObjectAddTransform(newObj, trans);
  GameObjectAddSprite(newObj, sprite);

  return newObj;
}

void GameStateLevel1MovementController(GameObjectPtr GOptr)
{
  PhysicsPtr physics = GameObjectGetPhysics(GOptr);

  TransformPtr transform = GameObjectGetTransform(GOptr);

  /* If either pointers are null, return. */
  if (!physics && !transform)
    return;

  Vector2D currentVelocity = *PhysicsGetVelocity(physics);

  /* Left and right input. */
  if (AEInputCheckCurr(VK_LEFT))
  {
    currentVelocity.x -= moveVelocity;
  }
  else if (AEInputCheckCurr(VK_RIGHT))
  {
    currentVelocity.x += moveVelocity;
  }
  else
  {
    currentVelocity.x = 0;
  }

  /* Jump input. */
  if (AEInputCheckCurr(VK_UP))
  {
    currentVelocity.y = jumpVelocity;
    PhysicsSetAcceleration(physics, &gravityNormal);
  }

  /* Check for landing. */
  Vector2D currentTranslation = *TransformGetTranslation(transform);
  /* If we are below the ground. */
  if (currentTranslation.y < groundHeight)
  {
    /* Pop into ground height. */
    currentTranslation.y = groundHeight;

    /* Set the translation. */
    TransformSetTranslation(transform, &currentTranslation);

    /* Turn off jump velocity. */
    currentVelocity.y = 0;

    /* Set physics acceleration. */
    PhysicsSetAcceleration(physics, &gravityNone);

    /* Take away life! */
    if (--numLives <= 0)
    {
      // Change state
      GameStateManagerSetNextState(GsLevel2);
    }
  }
  /* Set the physics to the new velocity. */
  PhysicsSetVelocity(physics, &currentVelocity);

}