//------------------------------------------------------------------------------
//
// File Name:	BehaviorBullet.h
// Author(s):	Doug Schilling (dschilling)
// Project:		MyGame
// Course:		CS230S19
//
// Copyright © 2019 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once
#include "Behavior.h"
//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Public Consts:
//------------------------------------------------------------------------------
typedef class BehaviorBullet* BehaviorBulletPtr;

class BehaviorBullet : public Behavior
{
public:
	BehaviorBullet(GameObjectPtr parent);
	virtual BehaviorPtr Clone(GameObjectPtr parent);
private:
	// Main interface with base class.
	virtual void Init();
	virtual void Update(float dt);
	virtual void Exit();

	// Private const
private:
	static const float bulletSpeedMax;
	// Maximum lifetime of a bullet (in seconds).
	static const float bulletLifeTimeMax;

	// Private structures
private:
	typedef enum BulletEnum
	{
		cBulletInvalid,
		cBulletIdle

	} BulletState;

	// Private functions
private:
	void UpdateLifeTimer(float dt);
	static void CollisionHandler(GameObjectPtr, GameObjectPtr);
};
const float BehaviorBullet::bulletSpeedMax = 500.0f;
// Maximum lifetime of a bullet (in seconds).
const float BehaviorBullet::bulletLifeTimeMax = 3.0f;
//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
