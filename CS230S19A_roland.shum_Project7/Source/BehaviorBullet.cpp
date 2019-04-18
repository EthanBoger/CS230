//------------------------------------------------------------------------------
//
// File Name:	BehaviorBullet.c
// Author(s):	Roland Shum
// Project:		MyGame
// Course:		CS230S19
//
// Copyright © 2019 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "BehaviorBullet.h"
#include "Behavior.h"
#include "GameObject.h"
#include "Collider.h"
#include "Teleporter.h"
//------------------------------------------------------------------------------
// Private Consts:
//------------------------------------------------------------------------------
const float BehaviorBullet::bulletSpeedMax = 500.0f;
// Maximum lifetime of a bullet (in seconds).
const float BehaviorBullet::bulletLifeTimeMax = 3.0f;
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

BehaviorBullet::BehaviorBullet(GameObjectPtr parent) : Behavior((int)cBulletInvalid, (int)cBulletIdle,
	parent)
{
	this->timer = bulletLifeTimeMax;
}

BehaviorBulletPtr BehaviorBullet::Clone(void) const
{
	BehaviorBulletPtr newbullet = new BehaviorBullet(*this);
	return newbullet;
}

// Initialize the current state of the behavior component.
// (Hint: Refer to the lecture notes on finite state machines (FSM).)
// Params:
//	 behavior = Pointer to the behavior component.
void BehaviorBullet::OnInit()
{
	if (this->stateCurr == cBulletIdle)
	{
		ColliderPtr parentCollider = this->parent->getCollider();
		if (parentCollider != NULL)
		{
			parentCollider->SetCollisionHandler(CollisionHandler);
		}
	}
}

// Update the current state of the behavior component.
// (Hint: Refer to the lecture notes on finite state machines (FSM).)
// Params:
//	 behavior = Pointer to the behavior component.
//	 dt = Change in time (in seconds) since the last game loop.
void BehaviorBullet::OnUpdate(float dt)
{
	switch (this->stateCurr)
	{
	case cBulletIdle:
		UpdateLifeTimer(dt);
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
void BehaviorBullet::OnExit()
{
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

void BehaviorBullet::UpdateLifeTimer(float dt)
{
  if (this == NULL)
    return;
  if (this->timer > 0)
  {
    // minus time
    this->timer -= dt;
    // If time is up
    if (this->timer <= 0)
    {
      //Destroy
		parent->Destroy();
    }
  }
}

void BehaviorBullet::CollisionHandler(GameObjectPtr objA, GameObjectPtr objB)
{
	// If both objects are valid.
	if (objA != NULL && objB != NULL)
	{
		if (strcmp(objB->getName(), "Asteroid") == 0)
		{
			objA->Destroy();
		}
	}
}