//------------------------------------------------------------------------------
//
// File Name:	Collider.c
// Author(s):	Roland Shum
// Project:		MyGame
// Course:		CS230S19
//
// Copyright © 2019 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Collider.h"
#include "Transform.h"
#include "GameObject.h"
#include "Vector2D.h"
#include <assert.h>
#include "ColliderCircle.h"
#include "ColliderLine.h"
//------------------------------------------------------------------------------
// Private Consts:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------
//typedef struct Collider
//{
//	// Pointer to the collider's parent game object.
//	GameObjectPtr parent;
//
//	// Pointer to a function that handles collisions between two objects.
//	CollisionEventHandler	handler;
//
//} Collider;
//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------
static bool ColliderIsColliding(ColliderPtr collider1, ColliderPtr collider2);
//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Dynamically allocate a new collider component.
// (Hint: Use calloc() to ensure that all member variables are initialized to 0.)
ColliderPtr ColliderCreate(GameObjectPtr parent)
{
	ColliderPtr collider = calloc(1, sizeof(Collider));
	if (collider == NULL)
		return NULL;
	collider->parent = parent;
	return collider;
}

// Dynamically allocate a clone of an existing collider component.
// (Hint: Perform a shallow copy of the member variables.)
// Params:
//	 other = Pointer to the component to be cloned.
//   parent = Pointer to the cloned component's parent.
// Returns:
//	 If 'other' is valid and the memory allocation was successful,
//	   then return a pointer to the cloned component,
//	   else return NULL.
ColliderPtr ColliderClone(const ColliderPtr other, GameObjectPtr parent)
{
	if (other == NULL)
		return NULL;

	// Create on the heap.
	ColliderPtr clone = calloc(1, other->memorySize);
	if (clone == NULL)
		return NULL;

	// Shallow copy
	memcpy_s(clone, other->memorySize, other, other->memorySize);

	// Now set the parent
	clone->parent = parent;

	return clone;
}

// Free the memory associated with a collider component.
// (Also, set the collider pointer to NULL.)
// Params:
//	 collider = Pointer to the collider component.
void ColliderFree(ColliderPtr * collider)
{
	free(*collider);
	*collider = NULL;
}

// Check if two objects are colliding.
// (Hint: Refer to the project instructions for implementation suggestions.)
// (Hint: Make sure to call the handler for both colliders, passing the 
//	  parent game object pointers in the correct order!)
// Params:
//	 collider1 = Pointer to the first collider component.
//	 collider2 = Pointer to the second collider component.
void ColliderCheck(ColliderPtr collider, ColliderPtr other)
{
	//// Check if both pointers are valid.
	if (collider == NULL || other == NULL)
		return;

	if (ColliderIsColliding(collider, other))
	{
		// We have collided.
		if (collider->handler != NULL)
			collider->handler(collider->parent, other->parent);
		if (other->handler != NULL)
			other->handler(other->parent, collider->parent);
	}
}

static bool ColliderIsColliding(ColliderPtr collider1, ColliderPtr collider2)
{
	if (collider1->type == ColliderTypeCircle && collider2->type == ColliderTypeCircle)
	{
		return ColliderCircleIsCollidingWithCircle(collider1, collider2);
	}
	else if (collider1->type == ColliderTypeLine && collider2->type == ColliderTypeLine)
	{
		return false;
	}
	// This and the last condition are the same, except swapped around.
	else if (collider1->type == ColliderTypeLine && collider2->type == ColliderTypeCircle)
	{
		return ColliderLineIsCollidingWithCircle(collider1, collider2);
	}
	else if (collider1->type == ColliderTypeCircle && collider2->type == ColliderTypeLine)
	{
		return ColliderLineIsCollidingWithCircle(collider2, collider1);
	}
	return false;
}

// Set the collision event handler for a collider.
// (Hint: This allows other components, such as behaviors, to respond to collision events.)
// (Note: It is acceptable for the handler to be NULL.  This allows an existing handler to be removed.)
// Params:
//	 collider = Pointer to the collider component.
//	 handler = Pointer to the collision event handler (may be NULL).
void ColliderSetCollisionHandler(ColliderPtr collider, CollisionEventHandler handler)
{
	collider->handler = handler;
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

