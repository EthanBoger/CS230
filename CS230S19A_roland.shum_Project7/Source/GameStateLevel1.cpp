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
enum MonkeyStates
{
  MonkeyInvalid = -1,
  MonkeyIdle,
  MonkeyWalk,
  MonkeyJump,

  MonkeyMax

};

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

  planetSpriteSource = new SpriteSource(1, 1, planetTexture);
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

  monkeySpriteSource[MonkeyIdle] = new SpriteSource(1, 1, monkeyTexture[MonkeyIdle]);
  if (monkeySpriteSource[MonkeyIdle] == NULL)
  {
    return;
  }

  monkeySpriteSource[MonkeyWalk] = new SpriteSource(3, 3, monkeyTexture[MonkeyWalk]);
  if (monkeySpriteSource[MonkeyWalk] == NULL)
  {
    return;
  }

  monkeySpriteSource[MonkeyJump] = new SpriteSource(1, 1, monkeyTexture[MonkeyJump]);
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

  if ((LivesTextSpriteSource = new SpriteSource(16, 6, LivesTextTexture)) == NULL)
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

  planetObj->Update(dt);
  LivesText->Update(dt);
  Monkey->Update(dt);

  planetObj->Draw();
  LivesText->Draw();
  Monkey->Draw();


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
  else if (AEInputCheckTriggered('4'))
  {
	  GameStateManagerSetNextState(GsOmega);
  }
}

// Shutdown any memory associated with the Level1 game state.
void GameStateLevel1Shutdown()
{
	delete planetObj;
	planetObj = NULL;
	delete Monkey;
	Monkey = NULL;
	delete LivesText;
	LivesText = NULL;
}

// Unload the resources associated with the Level1 game state.
void GameStateLevel1Unload()
{
  /* Free sprite source, texture, and texture list. */
  if (planetSpriteSource != NULL)
  {
    delete (planetSpriteSource);
	planetSpriteSource = NULL;
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
      delete (monkeySpriteSource[i]);
	  monkeySpriteSource[i] = NULL;
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
    delete (LivesTextSpriteSource);
  }
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

static GameObjectPtr GameStateLevel1CreatePlanet(void)
{
  GameObjectPtr newObj = new GameObject("Planet");
  TransformPtr trans = new Transform(0, 300);

  trans->setRotation(0);
  /* Set the scale by making a vector 2d of 100,100 */
  Vector2D vector_scale = { 100,100 };
  trans->setScale(&vector_scale);

  SpritePtr sprite = new Sprite("Planet Sprite");
  sprite->setMesh(Mesh1x1);
  sprite->setSpriteSource(planetSpriteSource);

  PhysicsPtr physics = new Physics();
  Vector2D vector_velo = { 150, 0 };
  physics->setVelocity(&vector_velo);
  physics->setAcceleration(&gravityNormal);

  newObj->addPhysics(physics);
  newObj->addTransform(trans);
  newObj->addSprite(sprite);

  return newObj;
}

static GameObjectPtr GameStateLevel1CreateMonkey(void)
{
  GameObjectPtr newObj = new GameObject("Monkey");
  TransformPtr trans = new Transform(0, groundHeight);

  trans->setRotation(0);
  /* Set the scale by making a vector 2d of 100,100 */
  Vector2D vector_scale = { 150,150 };
  trans->setScale(&vector_scale);

  SpritePtr sprite = new Sprite("Monkey Sprite");
  sprite->setMesh(mesh3x3);
  sprite->setSpriteSource(monkeySpriteSource[MonkeyIdle]);

  AnimationPtr anim = new Animation(sprite);
  PhysicsPtr physics = new Physics();

  newObj->addAnimation(anim);
  newObj->addPhysics(physics);
  newObj->addTransform(trans);
  newObj->addSprite(sprite);

  return newObj;
}

static GameObjectPtr GameStateLevel1CreateLivesText(void)
{
  GameObjectPtr newObj = new GameObject("LivesText");
  TransformPtr trans = new Transform(-350, 250);

  trans->setRotation(0);
  /* Set the scale by making a vector 2d of 100,100 */
  Vector2D vector_scale = { 50,50 }; 
  trans->setScale(&vector_scale);

  SpritePtr sprite = new Sprite("LivesText Sprite");
  sprite->setMesh(LivesTextVertexList);
  sprite->setSpriteSource(LivesTextSpriteSource);

  sprintf_s(livesBuffer, 16, "Lives: %d", numLives);
  sprite->setText(livesBuffer);

  newObj->addTransform(trans);
  newObj->addSprite(sprite);

  return newObj;
}

static void GameStateLevel1SetMonkeyState(GameObjectPtr gameObject, MonkeyStates newState)
{
  if (monkeyState != newState)
  {
    monkeyState = newState;

	SpritePtr sprite = gameObject->getSprite();
    AnimationPtr anim = gameObject->getAnimation();

    // Actually possible to just use SpriteSetMesh(sprite, monkeyTexture[newState]), 
    // but instructions oh well.
    switch (newState)
    {
    case MonkeyIdle:
		sprite->setMesh(Mesh1x1);
		sprite->setSpriteSource(monkeySpriteSource[MonkeyIdle]);
		anim->Play(1, 0.0f, false);
      break;
    case MonkeyJump:
		sprite->setMesh(Mesh1x1);
		sprite->setSpriteSource(monkeySpriteSource[MonkeyJump]);
		anim->Play(1, 0.0f, false);
      break;
    case MonkeyWalk:
		sprite->setMesh(mesh3x3);
		sprite->setSpriteSource(monkeySpriteSource[MonkeyWalk]);
		anim->Play(8, 0.05f, false);
      break;
    default:
        break;
    }
  }
}

void GameStateLevel1MovementController(GameObjectPtr GOptr)
{
	PhysicsPtr physics = GOptr->getPhysics();

	TransformPtr transform = GOptr->getTransform();

  /* If either pointers are null, return. */
  if (!physics && !transform)
    return;

  Vector2D currentVelocity = *physics->getVelocity();

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
	physics->setAcceleration(&gravityNormal);
  }

  /* Check for landing. */
  Vector2D currentTranslation = *transform->getTranslation();
  /* If we are below the ground. */
  if (currentTranslation.y < groundHeight)
  {
    /* Pop into ground height. */
    currentTranslation.y = groundHeight;

    /* Set the translation. */
	transform->setTranslation(&currentTranslation);

    /* Turn off jump velocity. */
    currentVelocity.y = 0;

    /* Set physics acceleration. */
	physics->setAcceleration(&gravityNone);

    GameStateLevel1SetMonkeyState(GOptr, MonkeyIdle);
  }
  /* Set the physics to the new velocity. */
  physics->setVelocity(&currentVelocity);

}

static void GameStateLevel1BounceController(GameObjectPtr gameObject)
{
	PhysicsPtr physics = gameObject->getPhysics();
	TransformPtr transform = gameObject->getTransform();

	/* If either pointers are null, return. */
	if (!physics && !transform)
		return;

	Vector2D currentPos = *transform->getTranslation();
	Vector2D currentVelocity = *physics->getVelocity();

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
  transform->setTranslation(&currentPos);
  physics->setVelocity(&currentVelocity);
}

static bool GameStateLevel1IsColliding(GameObjectPtr objectA, GameObjectPtr objectB)
{
  // Get the pos of two objs.
	TransformPtr transform_one = objectA->getTransform();
	TransformPtr transform_two = objectB->getTransform();

  // If distance is below the checkdistance
  if(Vector2DSquareDistance(transform_one->getTranslation(), transform_two->getTranslation()) < CheckSquareDistance)
  {
    return true;
  }
  else
  {
    return false;
  }
}