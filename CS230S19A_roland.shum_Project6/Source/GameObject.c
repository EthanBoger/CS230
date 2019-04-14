//------------------------------------------------------------------------------
//
// File Name:	GameObject.c
// Author(s):	Roland Shum
// Project:		MyGame
// Course:		CS230S19
//
// Copyright © 2019 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------
#include "stdafx.h"
#include "GameObject.h"

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Consts:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

// You are free to change the contents of this structure as long as you do not
//   change the public interface declared in the header.
typedef struct GameObject
{
	// The name of the game object.
	const char * name;

	// Flag to indicate that the game object is dead and should be destroyed.
	bool isDestroyed;

	// Pointer to an attached behavior component.
	BehaviorPtr behavior;

	// Pointer to an attached collider component.
	ColliderPtr collider;

	// Pointer to an attached physics component.
	PhysicsPtr physics;

	// Pointer to an attached sprite component.
	SpritePtr sprite;

	// Pointer to an attached transform component.
	TransformPtr transform;

	// Pointer to an attached animation component.
	AnimationPtr animation;

} GameObject;


//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Dynamically allocate a new game object.
// (Hint: Use calloc() to ensure that all member variables are initialized to 0.)
// Params:
//	 name = The name of the game object being created.
// Returns:
//	 If the memory allocation was successful,
//	   then return a pointer to the allocated memory,
//	   else return NULL.
GameObjectPtr GameObjectCreate(const char * name)
{
  GameObjectPtr result = calloc(1, sizeof(GameObject));
  /* Check for successful memory allocation. */
  if (result == NULL)
    return NULL;
  /* Set the name. */
  result->name = name;
  return result;
}

// Dynamically allocate a clone of an existing game object.
// (Hint: Make sure to perform a shallow copy or deep copy, as appropriate.)
// (WARNING: When you clone the behavior component you need to change the 'parent' variable.)
// Params:
//	 other = Pointer to the game object to be cloned.
// Returns:
//	 If 'other' is valid and the memory allocation was successful,
//	   then return a pointer to the cloned object,
//	   else return NULL.
GameObjectPtr GameObjectClone(const GameObjectPtr other)
{
  if (other == NULL)
    return NULL;

  GameObjectPtr result = calloc(1, sizeof(GameObject));
  /* Check for successful memory allocation. */
  if (result == NULL)
    return NULL;

  // Clone the gameobject.
  *result = *other;

  // Clone the components and set them.
  result->behavior = BehaviorClone(other->behavior, result);
  result->sprite = SpriteClone(other->sprite);
  result->animation = AnimationClone(other->animation, result->sprite);
  result->physics = PhysicsClone(other->physics);
  result->transform = TransformClone(other->transform);
  result->collider = ColliderClone(other->collider, result);

  return result;
}

// Flag a game object for destruction.
// (Note: This is to avoid game objects being destroyed while they are being processed.)
// Params:
//	 gameObject = Pointer to the game object to be destroyed.
// Returns:
//	 If 'gameObject' is valid,
//	   then set the 'isDestroyed' boolean variable,
//	   else bail.
void GameObjectDestroy(GameObjectPtr gameObject)
{
  if (gameObject != NULL) 
  {
    gameObject->isDestroyed = true;
  }
}

// Check whether a game object has been flagged for destruction.
// Params:
//	 gameObject = Pointer to the game object to be checked.
// Returns:
//	 If 'gameObject' is valid,
//	   then return the 'isDestroyed' boolean variable,
//	   else return false.
bool GameObjectIsDestroyed(GameObjectPtr gameObject)
{
  if (gameObject != NULL)
    return gameObject->isDestroyed;
  else
    return false;
}

// Free the memory associated with a game object, including all components.
// (Also, set the game object pointer to NULL.)
// Params:
//	 gameObject = Pointer to the game object pointer.
void GameObjectFree(GameObjectPtr * gameObject)
{
  if (*gameObject == NULL)
    return;

  /* Free the individual components. */
  PhysicsPtr physics = GameObjectGetPhysics(*gameObject);
  PhysicsFree(&physics);

  SpritePtr sprite = GameObjectGetSprite(*gameObject);
  SpriteFree(&sprite);

  TransformPtr transform = GameObjectGetTransform(*gameObject);
  TransformFree(&transform);

  AnimationPtr animation = GameObjectGetAnimation(*gameObject);
  AnimationFree(&animation);

  ColliderPtr collider = GameObjectGetCollider(*gameObject);
  ColliderFree(&collider);

  BehaviorPtr behavior = GameObjectGetBehavior(*gameObject);
  BehaviorFree(&behavior);

  free(*gameObject);
  *gameObject = NULL;
}

// Attach a physics component to a game object.
// Params:
//	 gameObject = Pointer to the game object.
//   physics = Pointer to the physics component to be attached.
void GameObjectAddPhysics(GameObjectPtr gameObject, PhysicsPtr physics)
{
  /* Check if pointer is not null. */
  if (gameObject)
  {
    gameObject->physics = physics;
  }
}

// Attach an animation component to a game object.
// Params:
//	 gameObject = Pointer to the game object.
//   animation = Pointer to the animation component to be attached.
void GameObjectAddAnimation(GameObjectPtr gameObject, AnimationPtr animation)
{
  if (gameObject && animation)
  {
    gameObject->animation = animation;
  }
}

// Attach a sprite component to a game object.
// Params:
//	 gameObject = Pointer to the game object.
//   sprite = Pointer to the sprite component to be attached.
void GameObjectAddSprite(GameObjectPtr gameObject, SpritePtr sprite)
{
  if (gameObject != NULL)
  {
    gameObject->sprite = sprite;
  }
}

// Attach a transform component to a game object.
// Params:
//	 gameObject = Pointer to the game object.
//   transform = Pointer to the transform component to be attached.
void GameObjectAddTransform(GameObjectPtr gameObject, TransformPtr transform)
{
  if (gameObject != NULL)
  {
    gameObject->transform = transform;
  }
}

// Attach a behavior component to a game object.
// Params:
//	 gameObject = Pointer to the game object.
//   behavior = Pointer to the behavior component to be attached.
void GameObjectAddBehavior(GameObjectPtr gameObject, BehaviorPtr behavior)
{
  if (gameObject != NULL)
  {
    // Set the behavior
    gameObject->behavior = behavior;
    // Set the parent
    behavior->parent = gameObject;
  }
}

// Attach a collider component to a game object.
// Params:
//	 gameObject = Pointer to the game object.
//   collider = Pointer to the collider component to be attached.
void GameObjectAddCollider(GameObjectPtr gameObject, ColliderPtr collider)
{
	if (gameObject != NULL)
	{
		collider->parent = gameObject;
		gameObject->collider = collider;
	}
}

// Get the physics component attached to a game object.
// Params:
//	 gameObject = Pointer to the game object.
// Returns:
//	 If the game object pointer is valid,
//		then return a pointer to the attached physics component,
//		else return NULL.
PhysicsPtr GameObjectGetPhysics(const GameObjectPtr gameObject)
{
  if (gameObject != NULL)
    return gameObject->physics;
  else
    return NULL;
}

// Get the animation component attached to a game object.
// Params:
//	 gameObject = Pointer to the game object.
// Returns:
//	 If the game object pointer is valid,
//		then return a pointer to the attached animation component,
//		else return NULL.
AnimationPtr GameObjectGetAnimation(const GameObjectPtr gameObject)
{
  if (gameObject != NULL)
    return gameObject->animation;
  else
    return NULL;
}

// Get the sprite component attached to a game object.
// Params:
//	 gameObject = Pointer to the game object.
// Returns:
//	 If the game object pointer is valid,
//		then return a pointer to the attached sprite component,
//		else return NULL.
SpritePtr GameObjectGetSprite(const GameObjectPtr gameObject)
{
  if (gameObject != NULL)
    return gameObject->sprite;
  else
    return NULL;
}

// Get the transform component attached to a game object.
// Params:
//	 gameObject = Pointer to the game object.
// Returns:
//	 If the game object pointer is valid,
//		then return a pointer to the attached transform component,
//		else return NULL.
TransformPtr GameObjectGetTransform(const GameObjectPtr gameObject)
{
  if (gameObject != NULL)
    return gameObject->transform;
  else
    return NULL;
}

// Get the collider component attached to a game object.
// Params:
//	 gameObject = Pointer to the game object.
// Returns:
//	 If the game object pointer is valid,
//		then return a pointer to the attached collider component,
//		else return NULL.
ColliderPtr GameObjectGetCollider(const GameObjectPtr gameObject)
{
	if (gameObject != NULL)
		return gameObject->collider;
	else
		return NULL;
}

// Get the behavior component attached to a game object.
// Params:
//	 gameObject = Pointer to the game object.
// Returns:
//	 If the game object pointer is valid,
//		then return a pointer to the attached behavior component,
//		else return NULL.
BehaviorPtr GameObjectGetBehavior(const GameObjectPtr gameObject)
{
  if (gameObject != NULL)
    return gameObject->behavior;
  else
    return NULL;
}

// Get the game object's name.
// Params:
//	 gameObject = Pointer to the game object.
// Returns:
//	 If the game object pointer is valid,
//		then return a pointer to the game object's name,
//		else return NULL.
const char * GameObjectGetName(const GameObject * gameObject)
{
  if (gameObject != NULL)
    return gameObject->name;
  else
    return NULL;
}

// Compare the game object's name with the specified name.
// Params:
//	 gameObject = Pointer to the game object.
//   name = Pointer to the name to be checked.
// Returns:
//	 If the game object pointer is valid,
//		then return true if the names match, false otherwise,
//		else return false.
bool GameObjectIsNamed(const GameObject * gameObject, const char * name)
{
  if (gameObject != NULL)
  {
    // Compare both strings. IF they are the same
    if (strcmp(gameObject->name, name) == 0)
    {
      return true;
    }
    // else return false
    return false;
  }
  else
    return false;
}

// Update any components attached to the game object.
// (Hint: You will need to call PhysicsUpdate().)
// (NOTE: You must first check for a valid pointer before calling this function.)
// Params:
//	 gameObject = Pointer to the game object.
//	 dt = Change in time (in seconds) since the last game loop.
void GameObjectUpdate(GameObjectPtr gameObject, float dt)
{
  if (gameObject != NULL)
  {
    if (gameObject->physics != NULL)
    {
      PhysicsUpdate(gameObject->physics, gameObject->transform, dt);
    }

    if (gameObject->animation != NULL)
    {
      AnimationUpdate(gameObject->animation, dt);
    }

    if (gameObject->behavior != NULL)
    {
      BehaviorUpdate(gameObject->behavior, dt);
    }
  }
}

// Draw any visible components attached to the game object.
// (Hint: You will need to call SpriteDraw(), passing the sprite and transform components.)
// (NOTE: You must first check for valid pointers before calling this function.)
// Params:
//	 gameObject = Pointer to the game object.
void GameObjectDraw(GameObjectPtr gameObject)
{
  /* Check if game object is null or its sprite. */
  if (gameObject != NULL && gameObject->sprite != NULL)
  {
    SpriteDraw(gameObject->sprite, gameObject->transform);
  }
}
