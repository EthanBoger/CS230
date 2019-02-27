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
//------------------------------------------------------------------------------
// Private Consts:
//------------------------------------------------------------------------------
// Maximum speed of the bullet.
static const float bulletSpeedMax = 500.0f;
// Maximum lifetime of a bullet (in seconds).
static const float bulletLifeTimeMax = 3.0f;

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------
typedef enum BulletEnum
{
  cBulletInvalid,
  cBulletIdle

} BulletState;
//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------
static void BehaviorBulletUpdateLifeTimer(BehaviorPtr behavior, float dt);
//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Dynamically allocate a new (Bullet) behavior component.
// (Hint: Use calloc() to ensure that all member variables are initialized to 0.)
BehaviorPtr BehaviorBulletCreate(void)
{
  BehaviorPtr newBehavior = calloc(1, sizeof(Behavior));
  if (newBehavior == NULL)
    return NULL;
  newBehavior->stateCurr = cBulletInvalid;
  newBehavior->stateNext = cBulletIdle;
  newBehavior->onInit = BehaviorBulletInit;
  newBehavior->onUpdate = BehaviorBulletUpdate;
  newBehavior->onExit = BehaviorBulletExit;
  newBehavior->timer = bulletLifeTimeMax;
  return newBehavior;
}

// Initialize the current state of the behavior component.
// (Hint: Refer to the lecture notes on finite state machines (FSM).)
// Params:
//	 behavior = Pointer to the behavior component.
void BehaviorBulletInit(BehaviorPtr behavior)
{
  UNREFERENCED_PARAMETER(behavior);
}

// Update the current state of the behavior component.
// (Hint: Refer to the lecture notes on finite state machines (FSM).)
// Params:
//	 behavior = Pointer to the behavior component.
//	 dt = Change in time (in seconds) since the last game loop.
void BehaviorBulletUpdate(BehaviorPtr behavior, float dt)
{
  switch (behavior->stateCurr)
  {
  case cBulletIdle:
    BehaviorBulletUpdateLifeTimer(behavior, dt);
    break;
  default:
    break;
  }
}

// Exit the current state of the behavior component.
// (Hint: Refer to the lecture notes on finite state machines (FSM).)
// Params:
//	 behavior = Pointer to the behavior component.
//	 dt = Change in time (in seconds) since the last game loop.
void BehaviorBulletExit(BehaviorPtr behavior)
{
  UNREFERENCED_PARAMETER(behavior);
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

static void BehaviorBulletUpdateLifeTimer(BehaviorPtr behavior, float dt)
{
  if (behavior == NULL)
    return;
  if (behavior->timer > 0)
  {
    // minus time
    behavior->timer -= dt;
    // If time is up
    if (behavior->timer <= 0)
    {
      //Destroy
      GameObjectDestroy(behavior->parent);
    }
  }
}