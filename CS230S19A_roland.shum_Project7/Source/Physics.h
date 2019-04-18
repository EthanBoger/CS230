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
#include "Vector2D.h"
#include "Component.h"
//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

typedef struct Transform * TransformPtr;

//------------------------------------------------------------------------------
// Public Constants:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

typedef struct Physics * PhysicsPtr;

// An example of the structure to be defined in Physics.c.
struct Physics : public Component
{
private:
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
public:
	Physics();
	Physics(const Physics& other) = default;
	~Physics() = default;

	const Vector2D * getAcceleration() const;
	const Vector2D * getVelocity() const;
	const Vector2D * getOldTranslation() const;
	float getRotationalVelocity() const;

	void setAcceleration(const Vector2D * acceleration);
	void setVelocity(const Vector2D * velocity);
	void setRotationalVelocity(float rotationalVelocity);
	void setDrag(float drag);

	void Update(float dt) override;
	void Draw() override;
	PhysicsPtr Clone(void) const;

};

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------


