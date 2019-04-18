//------------------------------------------------------------------------------
//
// File Name:	BehaviorSpaceship.h
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
#include <AEexport.h>
#include "Behavior.h"
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Consts:
//------------------------------------------------------------------------------
typedef class Spaceship* SpaceshipPtr;
//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------
class Spaceship : public Behavior
{
public:
	Spaceship(GameObjectPtr parent);
	virtual BehaviorPtr Clone(GameObjectPtr parent);
private:
	// Main interface with base class.
	virtual void Init();
	virtual void Update(float dt);
	virtual void Exit();
	
	// Private const
private:
	static const float spaceshipAcceleration;
	static const float spaceshipSpeedMax;
	static const float spaceshipTurnRateMax;
	static const float spaceshipWeaponCooldownTime;
	static const float spaceshipWeaponBulletSpeed;
	static const float spaceshipDeathDuration;

	// Private structures
private:
	typedef enum FSM
	{
		cSpaceshipInvalid,
		cSpaceshipIdle,
		cSpaceshipThrust,
		cSpaceshipDead
	} SpaceshipFSM;

	// Private functions
private:
	void UpdateRotation(float dt);
	void UpdateVelocity(float dt);
	void UpdateWeapon(float dt);
	void SpawnBullet();
	static void CollisionHandler(GameObjectPtr, GameObjectPtr);
	void DeadAnim(float dt);
};
const float Spaceship::spaceshipAcceleration = 150.0f;
const float Spaceship::spaceshipSpeedMax = 500.0f;
const float Spaceship::spaceshipTurnRateMax = PI / 1.5f;
const float Spaceship::spaceshipWeaponCooldownTime = 0.25f;
const float Spaceship::spaceshipWeaponBulletSpeed = 750.0f;
const float Spaceship::spaceshipDeathDuration = 3.0f;

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
