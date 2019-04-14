//------------------------------------------------------------------------------
//
// File Name:	Behavior.c
// Author(s):	Roland Shum
// Project:		MyGame
// Course:		CS230S19
//
// Copyright © 2019 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Behavior.h"

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

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Dynamically allocate a clone of an existing behavior.
// (Hint: Perform a shallow copy of the member variables, then change the 'parent' pointer.)
// Params:
//	 other = Pointer to the component to be cloned.
// Returns:
//	 If 'other' is valid and the memory allocation was successful,
//	   then return a pointer to the cloned component,
//	   else return NULL.
BehaviorPtr BehaviorClone(BehaviorPtr other, GameObjectPtr parent)
{
  if (other == NULL)
    return NULL;

  // Allocate memory for a new behavior
  BehaviorPtr clone = malloc(other->memorySize);

  // Check if clone was allocated
  if (clone == NULL)
  {
    return NULL;
  }

  // Shallow copy
  memcpy_s(clone, other->memorySize, other, other->memorySize);

  // Point the parent
  clone->parent = parent;

  // Return
  return clone;
}

// Free the memory associated with a behavior component.
// (Also, set the behavior pointer to NULL.)
// Params:
//	 behavior = Pointer to the behavior component.
void BehaviorFree(BehaviorPtr * behavior)
{
	if (*behavior == NULL)
		return;

  // Run the exit function if it has one.
  if ((*behavior)->onExit)
  {
    (*behavior)->onExit(*behavior);
  }

  // Free the mem and set to NULL
  free(*behavior);
  *behavior = NULL;
}

// Update the behavior component.
// (Hint: Refer to the Word document for detailed instructions regarding this function.)
// Params:
//	 behavior = Pointer to the behavior component.
//	 dt = Change in time (in seconds) since the last game loop.
void BehaviorUpdate(BehaviorPtr behavior, float dt)
{
  // Validate pointer
  if (behavior == NULL)
    return;

  // We are changing states if this is true.
  if (behavior->stateCurr != behavior->stateNext)
  {
    /* Exit the behavior if the function exists. */
    if (behavior->onExit)
    {
      behavior->onExit(behavior);
    }

    // Set the current state.
    behavior->stateCurr = behavior->stateNext;

    // Run the init behavior if it has one.
    if (behavior->onInit)
    {
      behavior->onInit(behavior);
    }
  }

  // Run the update function if it has one
  if (behavior->onUpdate)
  {
    behavior->onUpdate(behavior, dt);
  }
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

