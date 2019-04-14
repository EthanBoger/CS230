//------------------------------------------------------------------------------
//
// File Name:	GameObject.h
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

#include "Animation.h"
#include "Behavior.h"
#include "Collider.h"
#include "Physics.h"
#include "Sprite.h"
#include "Transform.h"
#include <memory>

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Constants:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

typedef struct GameObject GameObject;
typedef std::unique_ptr<GameObject *> GameObjectUPtr;
typedef std::shared_ptr<GameObject *> GameObjectSPtr;
typedef GameObject* GameObjectPtr;

// An example of the structure to be defined in GameObject.c.

// You are free to change the contents of this structure as long as you do not
//   change the public interface declared in the header.
struct GameObject
{
private:
	// The name of the game object.
	const char * name;

	// Flag to indicate that the game object is dead and should be destroyed.
	bool isDestroyed;

	// Pointer to an attached animation component.
	AnimationPtr animation;

	// Pointer to an attached behavior component.
	BehaviorPtr behavior;

	// Pointer to an attached collider component.
	ColliderPtr collider;

	// Pointer to an attached physics component.
	PhysicsPtr physics;

	// Pointer to an attached sprite component.
	SpritePtr sprite;

	// Pointer to an attached transform component.
	TransformPtr transform;

public:
	GameObject(const char *name);
	GameObject(const GameObject &copy);
	~GameObject();

	bool isDestroyed();
	void Destroy();

	void addPhysics(PhysicsPtr physics);
	void addAnimation(AnimationPtr animation);
	void addTransform(TransformPtr transform);
	void addBehavior(BehaviorPtr behavior);
	void addCollider(ColliderPtr collider);
	void addSprite(SpritePtr sprite);

	PhysicsPtr getPhysics() const;
	AnimationPtr getAnimation() const;
	TransformPtr getTransform()const;
	BehaviorPtr getBehavior() const;
	ColliderPtr getCollider() const;
	SpritePtr getSprite() const;

	const char * getName() const;
	bool isNamed(const char *name) const;

	void Update(float dt);
	void Draw() const;
};