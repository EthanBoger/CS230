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
static const float monkeyWalkSpeed = 500.0f;
static const float monkeyJumpVelocity = 1000.0f;
static const Vector2D gravityNormal = { 0.0f, -1500.0f };
static const Vector2D gravityNone = { 0.0f, 0.0f };

static const float wallDistance = 350.0f;
static const float CheckSquareDistance = (75.0f * 75.0f);

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------
typedef enum
{
  MonkeyInvalid = -1,
  MonkeyIdle,
  MonkeyWalk,
  MonkeyJump,

  MonkeyMax

} MonkeyStates;

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------
static int numLives = 0;

static AEGfxVertexList *Mesh1x1;
static AEGfxTexture *planetTexture;
static SpriteSourcePtr planetSpriteSource;
static GameObjectPtr planetObj;

static AEGfxVertexList *mesh3x3;
static AEGfxTexture *monkeyTexture [MonkeyMax];
static SpriteSourcePtr monkeySpriteSource[MonkeyMax];
static GameObjectPtr Monkey;

static AEGfxVertexList *LivesTextVertexList;
static AEGfxTexture *LivesTextTexture;
static SpriteSourcePtr LivesTextSpriteSource;
static GameObjectPtr LivesText;

static enum MonkeyStates monkeyState;
static char livesBuffer[16];
//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------
static GameObjectPtr GameStateLevel1CreatePlanet(void);
static GameObjectPtr GameStateLevel1CreateMonkey(void);
static GameObjectPtr GameStateLevel1CreateLivesText(void);
static void GameStateLevel1MovementController(GameObjectPtr GOptr);
static void GameStateLevel1SetMonkeyState(GameObjectPtr gameObject, MonkeyStates newState);
static void GameStateLevel1BounceController(GameObjectPtr gameObject);
static bool GameStateLevel1IsColliding(GameObjectPtr objectA, GameObjectPtr objectB);
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

	/* Close file. */
	fclose(numLivesfile);

  /* load planet stuff. */
  Mesh1x1 = MeshCreateQuad(0.5f, 0.5f, 1.0f, 1.0f, "Mesh1x1");
  if (Mesh1x1 == NULL)
  {
    return;
  }

  planetTexture = AEGfxTextureLoad("Assets/PlanetTexture.png");
  if (planetTexture == NULL)
  {
    return;
  }

  planetSpriteSource = SpriteSourceCreate(1, 1, planetTexture);
  if (planetSpriteSource == NULL)
  {
    return;
  }

  /* Load monkey stuff */
  mesh3x3 = MeshCreateQuad(0.5f, 0.5f, 1.0f / 3, 1.0f / 3, "Mesh3x3");
  if (mesh3x3 == NULL)
  {
    return;
  }
  
  if ((monkeyTexture[MonkeyIdle] = AEGfxTextureLoad("Assets/MonkeyIdle.png")) == NULL)
  {
    return;
  }

  if ((monkeyTexture[MonkeyWalk] = AEGfxTextureLoad("Assets/MonkeyWalk.png")) == NULL)
  {
    return;
  }
  
  if ((monkeyTexture[MonkeyJump] = AEGfxTextureLoad("Assets/MonkeyJump.png")) == NULL)
  {
    return;
  }

  monkeySpriteSource[MonkeyIdle] = SpriteSourceCreate(1, 1, monkeyTexture[MonkeyIdle]);
  if (monkeySpriteSource[MonkeyIdle] == NULL)
  {
    return;
  }

  monkeySpriteSource[MonkeyWalk] = SpriteSourceCreate(3, 3, monkeyTexture[MonkeyWalk]);
  if (monkeySpriteSource[MonkeyWalk] == NULL)
  {
    return;
  }

  monkeySpriteSource[MonkeyJump] = SpriteSourceCreate(1, 1, monkeyTexture[MonkeyJump]);
  if (monkeySpriteSource[MonkeyJump] == NULL)
  {
    return;
  }

  // Font loading.
  LivesTextVertexList = MeshCreateQuad(0.5f, 0.5f, 1.0f / 16, 1.0f / 6, "Mesh16x6");
  if (LivesTextVertexList == NULL)
  {
    return;
  }

  if ((LivesTextTexture = AEGfxTextureLoad("Assets/FontSheet.png")) == NULL)
  {
    return;
  }

  if ((LivesTextSpriteSource = SpriteSourceCreate(16, 6, LivesTextTexture)) == NULL)
  {
    return;
  }

}

// Initialize the memory associated with the Level1 game state.
void GameStateLevel1Init()
{
  // Create planet.
  planetObj = GameStateLevel1CreatePlanet();
  
  // Create monkey and set variables.
  Monkey = GameStateLevel1CreateMonkey();
  monkeyState = MonkeyInvalid;
  GameStateLevel1SetMonkeyState(Monkey, MonkeyIdle);

  // Lives text.
  LivesText = GameStateLevel1CreateLivesText();

  AEGfxSetBackgroundColor(1, 1, 1);
  AEGfxSetBlendMode(AE_GFX_BM_BLEND);
}

// Update the Level1 game state.
// Params:
//	 dt = Change in time (in seconds) since the last game loop.
void GameStateLevel1Update(float dt)
{
  GameStateLevel1MovementController(Monkey);
  GameStateLevel1BounceController(planetObj);

  GameObjectUpdate(planetObj, dt);
  GameObjectUpdate(LivesText, dt);
  GameObjectUpdate(Monkey, dt);

  GameObjectDraw(planetObj);
  GameObjectDraw(LivesText);
  GameObjectDraw(Monkey);

  // Check if colliding
  if (GameStateLevel1IsColliding(planetObj, Monkey))
  {
    numLives--;
    if (numLives <= 0)
    {
      GameStateManagerSetNextState(GsLevel2);
    }
    else
    {
      GameStateManagerSetNextState(GsRestart);
    }
  }

  /* Restart the scene if the user presses 1. */
  if (AEInputCheckTriggered('1'))
  {
    GameStateManagerSetNextState(GsRestart);
  }
  if (AEInputCheckTriggered('2'))
  {
    GameStateManagerSetNextState(GsLevel2);
  }
  if (AEInputCheckTriggered('3'))
  {
    GameStateManagerSetNextState(GsAsteroids);
  }
}

// Shutdown any memory associated with the Level1 game state.
void GameStateLevel1Shutdown()
{
  GameObjectFree(&planetObj);
  GameObjectFree(&Monkey);
  GameObjectFree(&LivesText);
}

// Unload the resources associated with the Level1 game state.
void GameStateLevel1Unload()
{
  /* Free sprite source, texture, and texture list. */
  if (planetSpriteSource != NULL)
  {
    SpriteSourceFree(&planetSpriteSource);
  }
  if (planetTexture != NULL)
  {
    AEGfxTextureUnload(planetTexture);
  }
  if (Mesh1x1 != NULL)
  {
    AEGfxMeshFree(Mesh1x1);
  }

  // Free monkey
  if (mesh3x3 != NULL)
  {
    AEGfxMeshFree(mesh3x3);
  }

  for (int i = 0; i < MonkeyMax; i++)
  {
    if (monkeyTexture[i] != NULL)
    {
      AEGfxTextureUnload(monkeyTexture[i]);
    }
    if (monkeySpriteSource[i] != NULL)
    {
      SpriteSourceFree(&monkeySpriteSource[i]);
    }
  }

  // Free text lives.
  if (LivesTextVertexList != NULL)
  {
    AEGfxMeshFree(LivesTextVertexList);
  }
  if (LivesTextTexture != NULL)
  {
    AEGfxTextureUnload(LivesTextTexture);
  }
  if (LivesTextSpriteSource != NULL)
  {
    SpriteSourceFree(&LivesTextSpriteSource);
  }
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

static GameObjectPtr GameStateLevel1CreatePlanet(void)
{
  GameObjectPtr newObj = GameObjectCreate("Planet");
  TransformPtr trans = TransformCreate(0, 300);

  /* Error checking. */
  if (trans == NULL)
    return NULL;

  TransformSetRotation(trans, 0);
  /* Set the scale by making a vector 2d of 100,100 */
  Vector2D vector_scale = { 100,100 };
  TransformSetScale(trans, &vector_scale);

  SpritePtr sprite = SpriteCreate("Planet Sprite");

  /* Error checking */
  if (sprite == NULL)
    return NULL;

  SpriteSetMesh(sprite, Mesh1x1);
  SpriteSetSpriteSource(sprite, planetSpriteSource);


  PhysicsPtr physics = PhysicsCreate();
  Vector2D vector_velo = { 150, 0 };

  PhysicsSetVelocity(physics, &vector_velo);
  PhysicsSetAcceleration(physics, &gravityNormal);

  /* Error checking */
  if (physics == NULL)
    return NULL;

  GameObjectAddPhysics(newObj, physics);
  GameObjectAddTransform(newObj, trans);
  GameObjectAddSprite(newObj, sprite);

  return newObj;
}

static GameObjectPtr GameStateLevel1CreateMonkey(void)
{
  GameObjectPtr newObj = GameObjectCreate("Monkey");
  TransformPtr trans = TransformCreate(0, groundHeight);

  /* Error checking. */
  if (trans == NULL)
    return NULL;

  TransformSetRotation(trans, 0);
  /* Set the scale by making a vector 2d of 100,100 */
  Vector2D vector_scale = { 150,150 };
  TransformSetScale(trans, &vector_scale);

  SpritePtr sprite = SpriteCreate("Monkey Sprite");

  /* Error checking */
  if (sprite == NULL)
    return NULL;

  SpriteSetMesh(sprite, mesh3x3);
  SpriteSetSpriteSource(sprite, monkeySpriteSource[MonkeyIdle]);

  AnimationPtr anim = AnimationCreate(sprite);
  PhysicsPtr physics = PhysicsCreate();

  /* Error checking */
  if (physics == NULL)
    return NULL;

  GameObjectAddAnimation(newObj, anim);
  GameObjectAddPhysics(newObj, physics);
  GameObjectAddTransform(newObj, trans);
  GameObjectAddSprite(newObj, sprite);

  return newObj;
}

static GameObjectPtr GameStateLevel1CreateLivesText(void)
{
  GameObjectPtr newObj = GameObjectCreate("LivesText");
  TransformPtr trans = TransformCreate(-350, 250);

  /* Error checking. */
  if (trans == NULL)
    return NULL;

  TransformSetRotation(trans, 0);
  /* Set the scale by making a vector 2d of 100,100 */
  Vector2D vector_scale = { 50,50 };
  TransformSetScale(trans, &vector_scale);

  SpritePtr sprite = SpriteCreate("LivesText Sprite");

  /* Error checking */
  if (sprite == NULL)
    return NULL;

  SpriteSetMesh(sprite, LivesTextVertexList);
  SpriteSetSpriteSource(sprite, LivesTextSpriteSource);

  sprintf_s(livesBuffer, 16, "Lives: %d", numLives);
  SpriteSetText(sprite, livesBuffer);

  GameObjectAddTransform(newObj, trans);
  GameObjectAddSprite(newObj, sprite);

  return newObj;
}

static void GameStateLevel1SetMonkeyState(GameObjectPtr gameObject, MonkeyStates newState)
{
  if (monkeyState != newState)
  {
    monkeyState = newState;

    SpritePtr sprite = GameObjectGetSprite(gameObject);
    AnimationPtr anim = GameObjectGetAnimation(gameObject);

    // Actually possible to just use SpriteSetMesh(sprite, monkeyTexture[newState]), 
    // but instructions oh well.
    switch (newState)
    {
    case MonkeyIdle:
      SpriteSetMesh(sprite, Mesh1x1);
      SpriteSetSpriteSource(sprite, monkeySpriteSource[MonkeyIdle]);
      AnimationPlay(anim, 1, 0.0f, false);
      break;
    case MonkeyJump:
      SpriteSetMesh(sprite, Mesh1x1);
      SpriteSetSpriteSource(sprite, monkeySpriteSource[MonkeyJump]);
      AnimationPlay(anim, 1, 0.0f, false);
      break;
    case MonkeyWalk:
      SpriteSetMesh(sprite, mesh3x3);
      SpriteSetSpriteSource(sprite, monkeySpriteSource[MonkeyWalk]);
      AnimationPlay(anim, 8, 0.05f, true);
      break;
    default:
        break;
    }
  }
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
    currentVelocity.x = -monkeyWalkSpeed;
    if (monkeyState != MonkeyJump)
    {
      GameStateLevel1SetMonkeyState(GOptr, MonkeyWalk);
    }
  }
  else if (AEInputCheckCurr(VK_RIGHT))
  {
    currentVelocity.x = monkeyWalkSpeed;
    if (monkeyState != MonkeyJump)
    {
      GameStateLevel1SetMonkeyState(GOptr, MonkeyWalk);
    }
  }
  else
  {
    if (monkeyState != MonkeyJump)
    {
      GameStateLevel1SetMonkeyState(GOptr, MonkeyIdle);
    }
    currentVelocity.x = 0;
  }

  /* Jump input. */
  if (AEInputCheckCurr(VK_UP))
  {
    GameStateLevel1SetMonkeyState(GOptr, MonkeyJump);

    currentVelocity.y = monkeyJumpVelocity;
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

    GameStateLevel1SetMonkeyState(GOptr, MonkeyIdle);
  }
  /* Set the physics to the new velocity. */
  PhysicsSetVelocity(physics, &currentVelocity);

}

static void GameStateLevel1BounceController(GameObjectPtr gameObject)
{
  PhysicsPtr physics = GameObjectGetPhysics(gameObject);
  TransformPtr transform = GameObjectGetTransform(gameObject);

  /* If either pointers are null, return. */
  if (!physics && !transform)
    return;

  Vector2D currentPos = *TransformGetTranslation(transform);
  Vector2D currentVelocity = *PhysicsGetVelocity(physics);

  // Handle x
  if (currentPos.x <= -wallDistance)
  {
    currentPos.x = -wallDistance;
    currentVelocity.x = -currentVelocity.x;
  }
  else if (currentPos.x >= wallDistance)
  {
    currentPos.x = wallDistance;
    currentVelocity.x = -currentVelocity.x;
  }
  
  // Handle y
  if (currentPos.y <= groundHeight)
  {
    // Conserve energy
    currentPos.y = groundHeight + (groundHeight - currentPos.y);
    currentVelocity.y = -currentVelocity.y;
  }
  TransformSetTranslation(transform, &currentPos);
  PhysicsSetVelocity(physics, &currentVelocity);
}

static bool GameStateLevel1IsColliding(GameObjectPtr objectA, GameObjectPtr objectB)
{
  // Get the pos of two objs.
  TransformPtr transform_one = GameObjectGetTransform(objectA);
  TransformPtr transform_two = GameObjectGetTransform(objectB);

  // If distance is below the checkdistance
  if(Vector2DSquareDistance(TransformGetTranslation(transform_one), TransformGetTranslation(transform_two)) < CheckSquareDistance)
  {
    return true;
  }
  else
  {
    return false;
  }
}