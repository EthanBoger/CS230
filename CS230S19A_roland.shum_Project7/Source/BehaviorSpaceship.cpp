//------------------------------------------------------------------------------
//
// File Name:	BehaviorSpaceships.c
// Author(s):	Roland Shum
// Project:		MyGame
// Course:		CS230S19
//
// Copyright © 2019 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "BehaviorSpaceship.h"
#include <AEEngine.h>
#include "Behavior.h"
#include "Physics.h"
#include "GameObject.h"
#include "Transform.h"
#include "Vector2D.h"
#include "GameObjectManager.h"
#include "Teleporter.h"
#include "GameStateManager.h"
//------------------------------------------------------------------------------
// Private Consts:
//------------------------------------------------------------------------------
const float Spaceship::spaceshipAcceleration = 150.0f;
const float Spaceship::spaceshipSpeedMax = 500.0f;
const float Spaceship::spaceshipTurnRateMax = PI / 1.5f;
const float Spaceship::spaceshipWeaponCooldownTime = 0.25f;
const float Spaceship::spaceshipWeaponBulletSpeed = 750.0f;
const float Spaceship::spaceshipDeathDuration = 3.0f;
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


// Dynamically allocate a new (Spaceship) behavior component.
// (Hint: Use calloc() to ensure that all member variables are initialized to 0.)

Spaceship::Spaceship(GameObjectPtr parent) : Behavior((int)cSpaceshipInvalid, (int)cSpaceshipIdle, parent)
{
	OnInit();
}

BehaviorPtr Spaceship::Clone(GameObjectPtr parent_l)
{
	SpaceshipPtr newAsteroird = new Spaceship(*this);
	newAsteroird->parent = parent_l;
	OnInit();
	return newAsteroird;
}


// Initialize the current state of the behavior component.
// (Hint: Refer to the lecture notes on finite state machines (FSM).)
// Params:
//	 behavior = Pointer to the behavior component.
void Spaceship::OnInit()
{
	if (this->stateCurr == cSpaceshipIdle)
	{
		ColliderPtr collider = this->parent->getCollider();
		collider->SetCollisionHandler(CollisionHandler);
	}
	else if (this->stateCurr == cSpaceshipDead)
	{
		this->timer = spaceshipDeathDuration;
	}
}

// Update the current state of the behavior component.
// (Hint: Refer to the lecture notes on finite state machines (FSM).)
// Params:
//	 behavior = Pointer to the behavior component.
//	 dt = Change in time (in seconds) since the last game loop.
void Spaceship::OnUpdate(float dt)
{
	switch (this->stateCurr)
	{
	case cSpaceshipIdle:
		UpdateRotation(dt);
		UpdateWeapon(dt);
		if (AEInputCheckCurr(VK_UP))
		{
			this->stateNext = cSpaceshipThrust;
		}
		break;
	case cSpaceshipThrust:
		UpdateRotation(dt);
		UpdateVelocity(dt);
		UpdateWeapon(dt);
		if (!AEInputCheckCurr(VK_UP))
		{
			this->stateNext = cSpaceshipIdle;
		}
		break;
	case cSpaceshipDead:
		this->timer -= dt;
		if (this->timer < 0)
			GameStateManagerSetNextState(GsRestart);
		DeadAnim(dt);
		break;
	default:
		break;
	}
	TeleporterUpdateObject(this->parent);
}

// Exit the current state of the behavior component.
// (Hint: Refer to the lecture notes on finite state machines (FSM).)
// Params:
//	 behavior = Pointer to the behavior component.
//	 dt = Change in time (in seconds) since the last game loop.
void Spaceship::OnExit()
{
}


//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

void Spaceship::UpdateRotation(float dt)
{
	UNREFERENCED_PARAMETER(dt);

	// Get physics component.
	PhysicsPtr physics = this->parent->getPhysics();

	if (AEInputCheckCurr(VK_LEFT))
	{
		physics->setRotationalVelocity(spaceshipTurnRateMax);
	}
	else if (AEInputCheckCurr(VK_RIGHT))
	{
		physics->setRotationalVelocity(-spaceshipTurnRateMax);
	}
	else
	{
		physics->setRotationalVelocity(0);
	}
}

void Spaceship::UpdateVelocity(float dt)
{
	UNREFERENCED_PARAMETER(dt);

	PhysicsPtr physics = this->parent->getPhysics();
	TransformPtr transform = this->parent->getTransform();

	if (physics == NULL || transform == NULL)
		return;

	// Get the unit vector rotation in Vector form.
	float rotation = transform->getRotation();

	Vector2D rotation2D;
	Vector2DFromAngleRad(&rotation2D, rotation);
	Vector2DNormalize(&rotation2D, &rotation2D);

	// Get a copy of the current velocity
	Vector2D velocity = *physics->getVelocity();

	// velocity += direction of rotation * spaceshipAcceleration * dt
	float accel = spaceshipAcceleration * dt;
	Vector2DScale(&rotation2D, &rotation2D, accel);
	Vector2DAdd(&velocity, &velocity, &rotation2D);

	float speed = Vector2DLength(&velocity);

	// Cap the max speed
	if (speed > spaceshipSpeedMax)
	{
		Vector2DScale(&velocity, &velocity, spaceshipSpeedMax / speed);
	}
	physics->setVelocity(&velocity);
}

void Spaceship::UpdateWeapon(float dt)
{
	if (this == NULL)
		return;
	if (this->timer > 0)
	{
		this->timer -= dt;
		if (this->timer < 0)
		{
			this->timer = 0;
		}
	}
	if (AEInputCheckCurr(' '))
	{
		if (this->timer <= 0)
		{
			SpawnBullet();
			this->timer = spaceshipWeaponCooldownTime;
		}
	}
}

void Spaceship::SpawnBullet()
{
	GameObjectPtr bullet = GameObjectManagers::getInstance().GetArchetype("Bullet");

	GameObjectPtr clone = new GameObject(*bullet);

	// Get the spaceship rotation and position
	TransformPtr transform = this->parent->getTransform();
	float rotation = transform->getRotation();
	Vector2D position = *transform->getTranslation();

	// Set the bullet transform
	TransformPtr bulletTransform = clone->getTransform();
	bulletTransform->setRotation(rotation);
	bulletTransform->setTranslation(&position);

	// Set the bullet physics stuff
	Vector2D rotationVector;
	Vector2DFromAngleRad(&rotationVector, rotation);

	PhysicsPtr bulletPhy = clone->getPhysics();
	Vector2DScale(&rotationVector, &rotationVector, spaceshipWeaponBulletSpeed);
	bulletPhy->setVelocity(&rotationVector);
	GameObjectManagers::getInstance().Add(clone);
}

void Spaceship::CollisionHandler(GameObjectPtr objA, GameObjectPtr objB)
{
	if (objA != NULL && objB != NULL)
	{
		if (objB->isNamed("Asteroid"))
		{
			objA->getBehavior()->stateNext = cSpaceshipDead;
		}
	}
}

void Spaceship::DeadAnim(float dt)
{
	UNREFERENCED_PARAMETER(dt);

	// Set the spaceship to spin increasingly
	PhysicsPtr physics = this->parent->getPhysics();
	physics->setRotationalVelocity(physics->getRotationalVelocity() + 10);

	// Make the spaceship smaller and smaller.
	TransformPtr transform = this->parent->getTransform();
	Vector2D newScale = *transform->getScale();
	Vector2DScale(&newScale, &newScale, 0.9f);
	transform->setScale(&newScale);
}