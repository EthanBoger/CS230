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
#include "Transform.h"
#include <AEEngine.h>
#include "GameObject.h"
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

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

Physics::Physics() : Component(Component::Physics, NULL)
{

}
// Get the acceleration of a physics component.
// Params:
//	 physics = Pointer to the physics component.
// Returns:
//	 If the physics pointer is valid,
//		then return a pointer to the component's acceleration structure,
//		else return a NULL pointer.
const Vector2D * Physics::getAcceleration() const
{
	return &this->acceleration;
}

// Get the velocity of a physics component.
// Params:
//	 physics = Pointer to the physics component.
// Returns:
//	 If the physics pointer is valid,
//		then return a pointer to the component's velocity structure,
//		else return a NULL pointer.
const Vector2D * Physics::getVelocity() const
{
	return &this->velocity;
}

// Get the old translation (position) of a physics component.
// Params:
//	 physics = Pointer to the physics component.
// Returns:
//	 If the physics pointer is valid,
//		then return a pointer to the component's oldTranslation structure,
//		else return a NULL pointer.
const Vector2D * Physics::getOldTranslation() const
{
	return &this->oldTranslation;
}

// Get the rotational velocity of a physics component.
// Params:
//	 physics = Pointer to the physics component.
// Returns:
//	 If the physics pointer is valid,
//		then return the component's rotational velocity value,
//		else return 0.0f.
float Physics::getRotationalVelocity() const
{
	return this->rotationalVelocity;
}

// Set the acceleration of a physics component.
// Params:
//	 physics = Pointer to the physics component.
//	 acceleration = Pointer to an acceleration vector.
void Physics::setAcceleration(const Vector2D * acceleration)
{
	this->acceleration = *acceleration;
}

// Set the velocity of a physics component.
// Params:
//	 physics = Pointer to the physics component.
//	 velocity = Pointer to a velocity vector.
void Physics::setVelocity(const Vector2D * velocity)
{
	this->velocity = *velocity;
}

// Set the rotational velocity of a physics component.
// Params:
//	 physics = Pointer to the physics component.
//	 rotationalVelocity = The new rotational velocity.
void Physics::setRotationalVelocity(float rotationalVelocity)
{
	this->rotationalVelocity = rotationalVelocity;
}

// Update the state of a physics component.
// (NOTE: This function must verify that the physics and transform pointers are valid.)
// Params:
//	 physics = Pointer to the physics component.
//	 transform = Pointer to the associated transform component.
//	 dt = Change in time (in seconds) since the last game loop.
void Physics::Update(float dt)
{
	TransformPtr transform = parent->getTransform();
	if (transform != NULL)
	{
		/* Update the old transform. */
		this->oldTranslation = *transform->getTranslation();

		/* Find the resulant translation after velocity and acceleration are applied, with DT. */
		Vector2D result, accel_dt, velo_dt;
		/* Find the acceleration after dt is applied. */
		Vector2DScale(&accel_dt, &this->acceleration, dt);

		/* Add that from the current velocity. */
		Vector2DAdd(&this->velocity, &this->velocity, &accel_dt);

		/* Add drag */
		if (this->drag != 0)
			Vector2DScale(&this->velocity, &this->velocity, this->drag);

		/* Find the current velocity after dt. */
		Vector2DScale(&velo_dt, &this->velocity, dt);

		/* Add that to the current translation. */
		Vector2DAdd(&result, &velo_dt, &this->oldTranslation);

		/* Set the new translation. */
		transform->setTranslation(&result);

		/* Update the rotation */
		float rotation = transform->getRotation() + this->rotationalVelocity * dt;

		/* Set the new rotation. */
		transform->setRotation(rotation);
	}
}

void Physics::Draw()
{

}

PhysicsPtr Physics::Clone(void) const
{
	return new Physics(*this);
}

// Set the "drag" value of a physics component.
// (NOTE: This value should be clamped between [0.0f, 1.0f].
// Params:
//	 physics = Pointer to the physics component.
//	 drag = The new drag value.
void Physics::setDrag(float drag)
{
	// Clamp the drag value
	AEClamp(drag, 0, 1);
	this->drag = drag;
}