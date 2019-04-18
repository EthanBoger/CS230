//------------------------------------------------------------------------------
//
// File Name:	BehaviorAsteroid.c
// Author(s):	Roland Shum
// Project:		MyGame
// Course:		CS230S19
//
// Copyright © 2019 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "BehaviorAsteroid.h"
#include "Behavior.h"
#include "Random.h"
#include "Collider.h"
#include "GameObject.h"
#include "Vector2D.h"
#include "Teleporter.h"
#include "GameStateAsteroids.h"
#include <AEEngine.h>
#include "ScoreSystem.h"
//------------------------------------------------------------------------------
// Private Consts:
//------------------------------------------------------------------------------
const float BehaviorAsteroid::asteroidSpeedMin = 50.0f;
const float BehaviorAsteroid::asteroidSpeedMax = 100.0f;
const float BehaviorAsteroid::asteroidSpawnOffset = 10.0f;
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

// Dynamically allocate a new (Asteroid) behavior component.
// (Hint: Use calloc() to ensure that all member variables are initialized to 0.)
BehaviorAsteroid::BehaviorAsteroid(GameObjectPtr parent) : Behavior((int)cAsteroidInvalid,
	(int)cAsteroidIdle, parent), size(cAsteroidLarge)
{
	OnInit();
}

// Initialize the current state of the behavior component.
// (Hint: Refer to the lecture notes on finite state machines (FSM).)
// Params:
//	 behavior = Pointer to the behavior component.
void BehaviorAsteroid::OnInit()
{
	if (this->stateCurr == cAsteroidIdle)
	{
		this->origin = static_cast<AsteroidOrigin>(RandomRange(0, 3));
		BehaviorAsteroid::SetPosition(this);
		BehaviorAsteroid::SetVelocity(this);

		ColliderPtr collider = this->parent->getCollider();
		if (collider != NULL)
		{
			collider->SetCollisionHandler(CollisionHandler);
		}
	}
}

BehaviorPtr BehaviorAsteroid::Clone(GameObjectPtr parent_l)
{
	BehaviorAsteroidPtr newAsteroird = new BehaviorAsteroid(*this);
	newAsteroird->parent = parent_l;
	OnInit();
	return newAsteroird;
}

// Update the current state of the behavior component.
// (Hint: Refer to the lecture notes on finite state machines (FSM).)
// Params:
//	 behavior = Pointer to the behavior component.
//	 dt = Change in time (in seconds) since the last game loop.
void BehaviorAsteroid::OnUpdate(float dt)
{
	UNREFERENCED_PARAMETER(dt);
	switch (this->stateCurr)
	{
	case cAsteroidIdle:
		
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
void BehaviorAsteroid::OnExit()
{
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------
void BehaviorAsteroid::SetPosition(BehaviorAsteroidPtr obj)
{
	if (obj->size == cAsteroidLarge)
	{
		// Zeroed new position.
		Vector2D newPosition;
		Vector2DZero(&newPosition);

		switch (obj->origin)
		{
		case cAsteroidOriginTlc:
			newPosition.x = AEGfxGetWinMinX() + asteroidSpawnOffset;
			newPosition.y = AEGfxGetWinMaxY() - asteroidSpawnOffset;
			break;
		case cAsteroidOriginTrc:
			newPosition.x = AEGfxGetWinMaxX() - asteroidSpawnOffset;
			newPosition.y = AEGfxGetWinMaxY() - asteroidSpawnOffset;
			break;
		case cAsteroidOriginBlc:
			newPosition.x = AEGfxGetWinMinX() + asteroidSpawnOffset;
			newPosition.y = AEGfxGetWinMinY() + asteroidSpawnOffset;
			break;
		case cAsteroidOriginBrc:
			newPosition.x = AEGfxGetWinMaxX() - asteroidSpawnOffset;
			newPosition.y = AEGfxGetWinMinY() + asteroidSpawnOffset;
			break;
		default:
			break;
		}
		obj->parent->getTransform()->setTranslation(&newPosition);
	}
}

void BehaviorAsteroid::SetVelocity(BehaviorAsteroidPtr obj)
{
	float newAngle = 0;
	if (obj->size == cAsteroidLarge)
	{
		switch (obj->origin)
		{
			case cAsteroidOriginTlc:
				//newAngle = RandomRangeFloat(-10, -80);
				newAngle = RandomRangeFloat(-80, -10);
				break;
			case cAsteroidOriginTrc:
				newAngle = RandomRangeFloat(-100, -170);
				break;
			case cAsteroidOriginBlc:
				newAngle = RandomRangeFloat(10, 80);
				break;
			case cAsteroidOriginBrc:
				newAngle = RandomRangeFloat(100, 170);
				break;
			default:
				break;
		}
	}
	else if (obj->size == cAsteroidMedium || obj->size == cAsteroidSmall)
	{
		newAngle = RandomRangeFloat(0, 359);
	}
	float newSpeed = RandomRangeFloat(asteroidSpeedMin, asteroidSpeedMax);
	PhysicsPtr physics = obj->parent->getPhysics();

	if (physics != NULL)
	{
		Vector2D newVelocity;
		// Get the degree in vector form.
		Vector2DFromAngleDeg(&newVelocity, newAngle);
		// Normalize it
		Vector2DNormalize(&newVelocity, &newVelocity);
		// Scale it so its speed is that.
		Vector2DScale(&newVelocity, &newVelocity, newSpeed);
		// Set the new velocity.
		physics->setVelocity(&newVelocity);
	}
}

void BehaviorAsteroid::CollisionHandler(GameObjectPtr objA, GameObjectPtr objB)
{
	if (objA != NULL && objB != NULL)
	{
		if (objA->isNamed("Spaceship") || objB->isNamed("Bullet"))
		{
			ScoreSystemIncreaseScore(20);
			objA->Destroy();
		}
	}
}