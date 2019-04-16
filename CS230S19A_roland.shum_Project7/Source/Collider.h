//------------------------------------------------------------------------------
//
// File Name:	Collider.h
// Author(s):	Doug Schilling (dschilling)
// Project:		Project 6
// Course:		CS230S19
//
// Copyright � 2019 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

typedef struct Collider * ColliderPtr;
//typedef struct GameObject * GameObjectPtr;

//------------------------------------------------------------------------------
// Public Consts:
//------------------------------------------------------------------------------

typedef enum ColliderType
{
	ColliderTypeNone,
	ColliderTypeCircle,
	ColliderTypeLine,

} ColliderType;

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

typedef void(*CollisionEventHandler)(GameObjectPtr gameObject1, GameObjectPtr gameObject2);

class Collider
{
protected:
	// Pointer to the collider's parent game object.
	GameObjectPtr parent;

	// The type of collider used by this component.
	// (Currently, Circle or Line).
	ColliderType type;

	// Pointer to a function that handles collisions between two objects.
	CollisionEventHandler	handler;

	// Dynamically allocate a clone of an existing collider component.
	// (Hint: Perform a shallow copy of the member variables.)
	// Params:
	//	 other = Pointer to the component to be cloned.
	//   parent = Pointer to the cloned component's parent.
	// Returns:
	//	 If 'other' is valid and the memory allocation was successful,
	//	   then return a pointer to the cloned component,
	//	   else return NULL.
	virtual ColliderPtr Clone(GameObjectPtr parent);

	// Set the collision event handler for a collider.
	// (Hint: This allows other components, such as behaviors, to respond to collision events.)
	// (Note: It is acceptable for the handler to be NULL.  This allows an existing handler to be removed.)
	// Params:
	//	 collider = Pointer to the collider component.
	//	 handler = Pointer to the collision event handler (may be NULL).
	void SetCollisionHandler(CollisionEventHandler handler);


public:
	// Check if two objects are colliding.
	// (Hint: Refer to the project instructions for implementation suggestions.)
	// (Hint: Make sure to call the handler for both colliders, passing the 
	//	  parent game object pointers in the correct order!)
	// Params:
	//	 collider1 = Pointer to the first collider component.
	//	 collider2 = Pointer to the second collider component.
	static void Check(ColliderPtr collider, ColliderPtr other);
};

//typedef struct Collider
//{
//	// Pointer to the collider's parent game object.
//	GameObjectPtr parent;
//
//	// The type of collider used by this component.
//	// (Currently, Circle or Line).
//	ColliderType type;
//
//	// Pointer to a function that handles collisions between two objects.
//	CollisionEventHandler	handler;
//
//	unsigned int	memorySize;
//
//} Collider;

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------