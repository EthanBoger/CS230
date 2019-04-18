//------------------------------------------------------------------------------
//
// File Name:	BehaviorAsteroid.h
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
#include "Behavior.h"
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

typedef struct Behavior * BehaviorPtr;

//------------------------------------------------------------------------------
// Public Consts:
//------------------------------------------------------------------------------

// An example of the enums to be defined in BehaviorAsteroid.c.


//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

typedef class BehaviorAsteroid* BehaviorAsteroidPtr;
class BehaviorAsteroid : public Behavior
{
private:
	typedef enum
	{
		cAsteroidLarge,
		cAsteroidMedium,
		cAsteroidSmall,

	} AsteroidSize;

	typedef enum
	{
		cAsteroidOriginTlc,
		cAsteroidOriginTrc,
		cAsteroidOriginBlc,
		cAsteroidOriginBrc,
		cAsteroidOriginCount,

	} AsteroidOrigin;

	typedef enum
	{
		cAsteroidInvalid,
		cAsteroidIdle
	} AsteroidStates;

private:
	static const float asteroidSpeedMin;
	static const float asteroidSpeedMax;
	static const float asteroidSpawnOffset;

	// Add asteroid-specific behavior variables.
	AsteroidSize	size;
	AsteroidOrigin	origin;


public:
	BehaviorAsteroid(GameObjectPtr parent);
	virtual BehaviorPtr Clone(GameObjectPtr parent);

private:
	virtual void OnInit();
	virtual void OnUpdate(float dt);
	virtual void OnExit();

	void SetPosition(BehaviorAsteroidPtr);
	void SetVelocity(BehaviorAsteroidPtr);
	static void CollisionHandler(GameObjectPtr, GameObjectPtr);
};
//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
