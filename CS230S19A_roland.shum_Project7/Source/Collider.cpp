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

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

Collider::Collider(ColliderType type) : type(type)
{

}

// Check if two objects are colliding.
// (Hint: Refer to the project instructions for implementation suggestions.)
// (Hint: Make sure to call the handler for both colliders, passing the 
//	  parent game object pointers in the correct order!)
// Params:
//	 collider1 = Pointer to the first collider component.
//	 collider2 = Pointer to the second collider component.
void Collider::Check(ColliderPtr collider, ColliderPtr other)
{
	//// Check if both pointers are valid.
	if (collider == NULL || other == NULL)
		return;

	if (Collider::IsColliding(collider, other))
	{
		// We have collided.
		if (collider->handler != NULL)
			collider->handler(collider->parent, other->parent);
		if (other->handler != NULL)
			other->handler(other->parent, collider->parent);
	}
}

bool Collider::IsColliding(ColliderPtr collider1, ColliderPtr collider2)
{
	if (collider1->type == ColliderTypeCircle && collider2->type == ColliderTypeCircle)
	{
		return ColliderCircle::CircleIsCollidingWithCircle(dynamic_cast<ColliderCirclePtr>(collider1), dynamic_cast<ColliderCirclePtr>(collider2));
	}
	else if (collider1->type == ColliderTypeLine && collider2->type == ColliderTypeLine)
	{
		return false;
	}
	// This and the last condition are the same, except swapped around.
	else if (collider1->type == ColliderTypeLine && collider2->type == ColliderTypeCircle)
	{
		return ColliderLine::LineIsCollidingWithCircle(dynamic_cast<ColliderLinePtr>(collider1), dynamic_cast<ColliderCirclePtr>(collider2));
	}
	else if (collider1->type == ColliderTypeCircle && collider2->type == ColliderTypeLine)
	{
		return ColliderLine::LineIsCollidingWithCircle(dynamic_cast<ColliderLinePtr>(collider2), dynamic_cast<ColliderCirclePtr>(collider1));
	}
	return false;
}

// Set the collision event handler for a collider.
// (Hint: This allows other components, such as behaviors, to respond to collision events.)
// (Note: It is acceptable for the handler to be NULL.  This allows an existing handler to be removed.)
// Params:
//	 collider = Pointer to the collider component.
//	 handler = Pointer to the collision event handler (may be NULL).
void Collider::SetCollisionHandler(CollisionEventHandler handler)
{
	this->handler = handler;
}

void Collider::SetParent(GameObjectPtr parent)
{
	this->parent = parent;
}
//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

