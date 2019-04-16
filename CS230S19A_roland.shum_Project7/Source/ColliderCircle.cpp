//------------------------------------------------------------------------------
//
// File Name:	Stub.c
// Author(s):	Roland Shum
// Project:		MyGame
// Course:		CS230S19
//
// Copyright © 2019 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Stub.h"

#include "ColliderCircle.h"
#include "Vector2D.h"
#include "Transform.h"
#include "GameObject.h"

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

// Dynamically allocate a new (circle) collider component.
// (Hint: Use calloc() to ensure that all member variables are initialized to 0.)
// (Hint: Make sure to initialize the ColliderType and memorySize correctly.)
ColliderCircle::ColliderCircle(void) : Collider(ColliderTypeCircle)
{
}

// Get the circle collider's radius.
// Params:
//	 collider = Pointer to the circle collider component.
// Returns:
//	 If the pointer is valid,
//	   then return the circle collider's radius,
//	   else return 0.0f.
float ColliderCircle::GetRadius()
{
	return this->radius;
}

// Set the circle collider's radius.
// Params:
//	 collider = Pointer to the circle collider component.
//   radius = the circle collider's new radius.
void ColliderCircle::SetRadius(float radius)
{
	this->radius = radius;
}

// Check for collision between two circle colliders.
// Params:
//	 collider1 = Pointer to the first circle collider component.
//	 collider2 = Pointer to the second circle collider component.
bool ColliderCircle::CircleIsCollidingWithCircle(ColliderCirclePtr collider, ColliderCirclePtr other)
{
	// Check for null
	if (collider == NULL || other == NULL)
		return false;

	// Check if both are circle
	if (collider->type != ColliderTypeCircle && other->type != ColliderTypeCircle)
		return false;

	// Get the distance squared between them
	TransformPtr thisTransform, otherTransform;
	thisTransform = GameObjectGetTransform(thisCircle->base.parent);
	otherTransform = GameObjectGetTransform(otherCircle->base.parent);
	float distanceSquared = Vector2DSquareDistance(thisTransform->getTranslation(), otherTransform->getTranslation());

	// Get the sum of their radius, then sqaure it.
	float radius = thisCircle->radius + otherCircle->radius;
	float radiusSquare = radius * radius;

	// Compare the two.
	if (radiusSquare > distanceSquared)
	{
		// We have collided.
		if (collider->handler != NULL)
			collider->handler(collider->parent, other->parent);
		if (other->handler != NULL)
			other->handler(other->parent, collider->parent);
		return true;
	}
	return false;
}

ColliderPtr ColliderCircle::Clone(GameObjectPtr parent)
{
	ColliderCirclePtr newLine = new ColliderCircle(*this);
	newLine->parent = parent;
	return newLine;
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

