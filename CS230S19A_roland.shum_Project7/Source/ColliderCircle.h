//------------------------------------------------------------------------------
//
// File Name:	ColliderCircle.h
// Author(s):	Doug Schilling (dschilling)
// Project:		Project 6
// Course:		CS230S19
//
// Copyright © 2019 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------
#include "Collider.h"
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Consts:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------
typedef ColliderCircle* ColliderCirclePtr;
// An example of the structure to be defined in ColliderCircle.c.

struct ColliderCircle : public Collider
{
public:
	ColliderCircle();

	// Get the circle collider's radius.
	// Params:
	//	 collider = Pointer to the circle collider component.
	// Returns:
	//	 If the pointer is valid,
	//	   then return the circle collider's radius,
	//	   else return 0.0f.
	float GetRadius();

	// Set the circle collider's radius.
	// Params:
	//	 collider = Pointer to the circle collider component.
	//   radius = the circle collider's new radius.
	void SetRadius(float radius);

	static bool CircleIsCollidingWithCircle(ColliderCirclePtr collider, ColliderCirclePtr other);

	virtual ColliderPtr Clone(GameObjectPtr parent);
private:
	// Radius of the circle collider.
	float radius;

};

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
