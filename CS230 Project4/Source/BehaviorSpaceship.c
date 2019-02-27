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
//------------------------------------------------------------------------------
// Private Consts:
//------------------------------------------------------------------------------
static const float spaceshipAcceleration = 150.0f;
static const float spaceshipSpeedMax = 500.0f;
static const float spaceshipTurnRateMax = PI / 1.5f;
static const float spaceshipWeaponCooldownTime = 0.034f;
static const float spaceshipWeaponBulletSpeed = 750.0f;

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------
typedef enum FSM
{
  cSpaceshipInvalid,
  cSpaceshipIdle,
  cSpaceshipThrust
}SpaceshipFSM;
//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------
static void BehaviorSpaceshipUpdateRotation(BehaviorPtr behavior, float dt);
static void BehaviorSpaceshipUpdateVelocity(BehaviorPtr behavior, float dt);
static void BehaviorSpaceshipUpdateWeapon(BehaviorPtr behavior, float dt);
static void BehaviorSpaceshipSpawnBullet(BehaviorPtr behavior);

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------


// Dynamically allocate a new (Spaceship) behavior component.
// (Hint: Use calloc() to ensure that all member variables are initialized to 0.)
BehaviorPtr BehaviorSpaceshipCreate(void)
{
  BehaviorPtr newBehavior = calloc(1, sizeof(Behavior));
  if (newBehavior == NULL)
    return NULL;

  newBehavior->stateCurr = cSpaceshipInvalid;
  newBehavior->stateNext = cSpaceshipIdle;
  newBehavior->onInit = BehaviorSpaceshipInit;
  newBehavior->onUpdate = BehaviorSpaceshipUpdate;
  newBehavior->onExit = BehaviorSpaceshipExit;
  return newBehavior;
}

// Initialize the current state of the behavior component.
// (Hint: Refer to the lecture notes on finite state machines (FSM).)
// Params:
//	 behavior = Pointer to the behavior component.
void BehaviorSpaceshipInit(BehaviorPtr behavior)
{
  UNREFERENCED_PARAMETER(behavior);
}

// Update the current state of the behavior component.
// (Hint: Refer to the lecture notes on finite state machines (FSM).)
// Params:
//	 behavior = Pointer to the behavior component.
//	 dt = Change in time (in seconds) since the last game loop.
void BehaviorSpaceshipUpdate(BehaviorPtr behavior, float dt)
{
  switch (behavior->stateCurr)
  {
  case cSpaceshipIdle:
    BehaviorSpaceshipUpdateRotation(behavior, dt);
    BehaviorSpaceshipUpdateWeapon(behavior, dt);
    if (AEInputCheckCurr(VK_UP))
    {
      behavior->stateNext = cSpaceshipThrust;
    }
    break;
  case cSpaceshipThrust:
    BehaviorSpaceshipUpdateRotation(behavior, dt);
    BehaviorSpaceshipUpdateVelocity(behavior, dt);
    BehaviorSpaceshipUpdateWeapon(behavior, dt);
    if (!AEInputCheckCurr(VK_UP))
    {
      behavior->stateNext = cSpaceshipIdle;
    }
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
void BehaviorSpaceshipExit(BehaviorPtr behavior)
{
  UNREFERENCED_PARAMETER(behavior);
}


//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

static void BehaviorSpaceshipUpdateRotation(BehaviorPtr behavior, float dt)
{
  UNREFERENCED_PARAMETER(dt);

  // Get physics component.
  PhysicsPtr physics = GameObjectGetPhysics(behavior->parent);

  if (AEInputCheckCurr(VK_LEFT))
  {
    PhysicsSetRotationalVelocity(physics, spaceshipTurnRateMax);
  }
  else if (AEInputCheckCurr(VK_RIGHT))
  {
    PhysicsSetRotationalVelocity(physics, -spaceshipTurnRateMax);
  }
  else
  {
    PhysicsSetRotationalVelocity(physics, 0);
  }
}

static void BehaviorSpaceshipUpdateVelocity(BehaviorPtr behavior, float dt)
{
  PhysicsPtr physics = GameObjectGetPhysics(behavior->parent);
  TransformPtr transform = GameObjectGetTransform(behavior->parent);

  if (physics == NULL || transform == NULL)
    return;

  // Get the unit vector rotation in Vector form.
  float rotation = TransformGetRotation(transform); 

  Vector2D rotation2D;
  Vector2DFromAngleRad(&rotation2D, rotation);
  Vector2DNormalize(&rotation2D, &rotation2D);

  // Get a copy of the current velocity
  Vector2D velocity;
  velocity = *PhysicsGetVelocity(physics);

  // velocity += direction of rotation * spaceshipAcceleration * dt
  float accel = spaceshipAcceleration * dt;
  Vector2DScale(&rotation2D, &rotation2D, accel);
  Vector2DAdd(&velocity, &velocity, &rotation2D);

  float speed = Vector2DLength(&velocity);

  if (speed > spaceshipSpeedMax)
  {
    Vector2DScale(&velocity, &velocity, spaceshipSpeedMax / speed);
  }
  PhysicsSetVelocity(physics, &velocity);
}

static void BehaviorSpaceshipUpdateWeapon(BehaviorPtr behavior, float dt)
{
  if (behavior == NULL)
    return;
  if (behavior->timer > 0)
  {
    behavior->timer -= dt;
    if (behavior->timer < 0)
    {
      behavior->timer = 0;
    }
  }
  if (AEInputCheckCurr(' '))
  {
    if (behavior->timer <= 0)
    {
      BehaviorSpaceshipSpawnBullet(behavior);
      behavior->timer = spaceshipWeaponCooldownTime;
    }
  }
}

static void BehaviorSpaceshipSpawnBullet(BehaviorPtr behavior)
{
  GameObjectPtr bullet = GameObjectManagerGetArchetype("Bullet");

  if (bullet == NULL)
    return;

  GameObjectPtr clone = GameObjectClone(bullet);

  // Get the spaceship rotation and position
  TransformPtr transform = GameObjectGetTransform(behavior->parent);
  float rotation = TransformGetRotation(transform);
  Vector2D position = *TransformGetTranslation(transform);

  // Set the bullet transform
  TransformPtr bulletTransform = GameObjectGetTransform(clone);
  TransformSetRotation(bulletTransform, rotation);
  TransformSetTranslation(bulletTransform, &position);

  // Set the bullet physics stuff
  Vector2D rotationVector;
  Vector2DFromAngleRad(&rotationVector, rotation);
  PhysicsPtr bulletPhy = GameObjectGetPhysics(clone);
  Vector2DScale(&rotationVector, &rotationVector, spaceshipWeaponBulletSpeed);
  PhysicsSetVelocity(bulletPhy, &rotationVector);

  GameObjectManagerAdd(clone);
}