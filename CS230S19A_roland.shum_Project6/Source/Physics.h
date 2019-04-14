//------------------------------------------------------------------------------
//
// File Name:	Physics.h
// Author(s):	Doug Schilling (dschilling)
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

//------------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
	/* Assume C declarations for C++ */
#endif

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

typedef struct Transform * TransformPtr;
typedef struct Vector2D Vector2D;

//------------------------------------------------------------------------------
// Public Constants:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

typedef struct Physics * PhysicsPtr;

// An example of the structure to be defined in Physics.c.
#if 0
typedef struct Physics
{
	// Previous position.  May be used for resolving collisions.
	Vector2D	oldTranslation;

	// Acceleration = inverseMass * (sum of forces)
	Vector2D	acceleration;

	// Velocity may be stored as a direction vector and speed scalar, instead.
	Vector2D	velocity;

	// Rotational velocity (in radians).
	float rotationalVelocity;

	// Drag to be applied to moving objects in order to slow them down.
	float drag;

	// Used when calculating acceleration due to forces.
	// Used when resolving collision between two dynamic objects.
	float		inverseMass;

} Physics;
#endif

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Dynamically allocate a new physics component.
// (Hint: Use calloc() to ensure that all member variables are initialized to 0.)
PhysicsPtr PhysicsCreate(void);

// Dynamically allocate a clone of an existing physics component.
// (Hint: Perform a shallow copy of the member variables.)
// Params:
//	 other = Pointer to the component to be cloned.
// Returns:
//	 If 'other' is valid and the memory allocation was successful,
//	   then return a pointer to the cloned component,
//	   else return NULL.
PhysicsPtr PhysicsClone(const PhysicsPtr other);

// Free the memory associated with a physics component.
// (Also, set the physics pointer to NULL.)
// Params:
//	 physics = Pointer to the physics pointer.
void PhysicsFree(PhysicsPtr * physics);

// Get the acceleration of a physics component.
// Params:
//	 physics = Pointer to the physics component.
// Returns:
//	 If the physics pointer is valid,
//		then return a pointer to the component's acceleration structure,
//		else return a NULL pointer.
const Vector2D * PhysicsGetAcceleration(const PhysicsPtr physics);

// Get the velocity of a physics component.
// Params:
//	 physics = Pointer to the physics component.
// Returns:
//	 If the physics pointer is valid,
//		then return a pointer to the component's velocity structure,
//		else return a NULL pointer.
const Vector2D * PhysicsGetVelocity(const PhysicsPtr physics);

// Get the rotational velocity of a physics component.
// Params:
//	 physics = Pointer to the physics component.
// Returns:
//	 If the physics pointer is valid,
//		then return the component's rotational velocity value,
//		else return 0.0f.
float PhysicsGetRotationalVelocity(PhysicsPtr physics);

// Get the old translation (position) of a physics component.
// Params:
//	 physics = Pointer to the physics component.
// Returns:
//	 If the physics pointer is valid,
//		then return a pointer to the component's oldTranslation structure,
//		else return a NULL pointer.
const Vector2D * PhysicsGetOldTranslation(PhysicsPtr physics);

// Set the acceleration of a physics component.
// Params:
//	 physics = Pointer to the physics component.
//	 acceleration = Pointer to an acceleration vector.
void PhysicsSetAcceleration(PhysicsPtr physics, const Vector2D * acceleration);

// Set the velocity of a physics component.
// Params:
//	 physics = Pointer to the physics component.
//	 velocity = Pointer to a velocity vector.
void PhysicsSetVelocity(PhysicsPtr physics, const Vector2D * velocity);

// Set the rotational velocity of a physics component.
// Params:
//	 physics = Pointer to the physics component.
//	 rotationalVelocity = The new rotational velocity.
void PhysicsSetRotationalVelocity(PhysicsPtr physics, float rotationalVelocity);

// Set the "drag" value of a physics component.
// (NOTE: This value should be clamped between [0.0f, 1.0f].
// Params:
//	 physics = Pointer to the physics component.
//	 drag = The new drag value.
void PhysicsSetDrag(PhysicsPtr physics, float drag);

// Update the state of a physics component using the Semi-Implicit Euler method,
//	 as outlined in the "Dynamics" lecture slides.
// (NOTE: This function must verify that the physics and transform pointers are valid.)
// Params:
//	 physics = Pointer to the physics component.
//	 transform = Pointer to the associated transform component.
//	 dt = Change in time (in seconds) since the last game loop.
void PhysicsUpdate(PhysicsPtr physics, TransformPtr transform, float dt);

//------------------------------------------------------------------------------

#ifdef __cplusplus
}                       /* End of extern "C" { */
#endif

