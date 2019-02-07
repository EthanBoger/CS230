//------------------------------------------------------------------------------
//
// File Name:	Physics.c
// Author(s):	Roland Shum
// Project:		MyGame
// Course:		CS230S19
//
// Copyright © 2019 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once
#include "stdafx.h"
#include "Physics.h"
#include "Vector2D.h"
#include "Transform.h"
//------------------------------------------------------------------------------
// Include Files:
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

typedef struct Physics * PhysicsPtr;

typedef struct Physics
{
	// Previous position.  May be used for resolving collisions.
	Vector2D	oldTranslation;

	// Acceleration = inverseMass * (sum of forces)
	Vector2D	acceleration;

	// Velocity may be stored as a direction vector and speed scalar, instead.
	Vector2D	velocity;

	// Used when calculating acceleration due to forces.
	// Used when resolving collision between two dynamic objects.
	float		inverseMass;

} Physics;

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Dynamically allocate a new physics component.
// (Hint: Use calloc() to ensure that all member variables are initialized to 0.)
PhysicsPtr PhysicsCreate(void)
{
  return calloc(1, sizeof(Physics));
}

// Free the memory associated with a physics component.
// (Also, set the physics pointer to NULL.)
// Params:
//	 physics = Pointer to the physics pointer.
void PhysicsFree(PhysicsPtr * physics)
{
  if (*physics == NULL)
    return;
  free(*physics);
  *physics = NULL;
}

// Get the acceleration of a physics component.
// Params:
//	 physics = Pointer to the physics component.
// Returns:
//	 If the physics pointer is valid,
//		then return a pointer to the component's acceleration structure,
//		else return a NULL pointer.
const Vector2D * PhysicsGetAcceleration(const PhysicsPtr physics)
{
  if (physics != NULL)
    return &physics->acceleration;
  else
    return NULL;
}

// Get the velocity of a physics component.
// Params:
//	 physics = Pointer to the physics component.
// Returns:
//	 If the physics pointer is valid,
//		then return a pointer to the component's velocity structure,
//		else return a NULL pointer.
const Vector2D * PhysicsGetVelocity(const PhysicsPtr physics)
{
  if (physics != NULL)
    return &physics->velocity;
  else
    return NULL;
}

// Get the old translation (position) of a physics component.
// Params:
//	 physics = Pointer to the physics component.
// Returns:
//	 If the physics pointer is valid,
//		then return a pointer to the component's oldTranslation structure,
//		else return a NULL pointer.
const Vector2D * PhysicsGetOldTranslation(PhysicsPtr physics)
{
  if (physics != NULL)
    return &physics->oldTranslation;
  else
    return NULL;
}

// Set the acceleration of a physics component.
// Params:
//	 physics = Pointer to the physics component.
//	 acceleration = Pointer to an acceleration vector.
void PhysicsSetAcceleration(PhysicsPtr physics, const Vector2D * acceleration)
{
  if (physics != NULL)
    physics->acceleration = *acceleration;
}

// Set the velocity of a physics component.
// Params:
//	 physics = Pointer to the physics component.
//	 velocity = Pointer to a velocity vector.
void PhysicsSetVelocity(PhysicsPtr physics, const Vector2D * velocity)
{
  if (physics != NULL)
    physics->velocity = *velocity;
}

// Update the state of a physics component.
// (NOTE: This function must verify that the physics and transform pointers are valid.)
// Params:
//	 physics = Pointer to the physics component.
//	 transform = Pointer to the associated transform component.
//	 dt = Change in time (in seconds) since the last game loop.
void PhysicsUpdate(PhysicsPtr physics, TransformPtr transform, float dt)
{
  if(physics != NULL && transform != NULL)
  { 
    /* Update the old transform. */
    physics->oldTranslation = *TransformGetTranslation(transform);

    /* Find the resulant translation after velocity and acceleration are applied, with DT. */
    Vector2D result, accel_dt, velo_dt;
    /* Find the acceleration after dt is applied. */
    Vector2DScale(&accel_dt, &physics->acceleration, dt);
    /* Subtract that from the current velocity. */
    Vector2DAdd(&physics->velocity, &physics->velocity, &accel_dt);

    /* Find the current velocity after dt. */
    Vector2DScale(&velo_dt, &physics->velocity, dt);
    /* Add that to the current translation. */
    Vector2DAdd(&result, &velo_dt, &physics->oldTranslation);

    /* Set the new translation. */
    TransformSetTranslation(transform, &result);
  }
}
