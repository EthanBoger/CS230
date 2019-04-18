//------------------------------------------------------------------------------
//
// File Name:	Collider.h
// Author(s):	Doug Schilling (dschilling)
// Project:		Project 6
// Course:		CS230S19
//
// Copyright © 2019 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once
#include "Component.h"
//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

typedef class Collider * ColliderPtr;
typedef struct GameObject * GameObjectPtr;

//------------------------------------------------------------------------------
// Public Consts:
//------------------------------------------------------------------------------

typedef enum ColliderType
{
	ColliderTypeNone,
	ColliderTypeCircle,
	ColliderTypeLine,

} ColliderType;

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

typedef void(*CollisionEventHandler)(GameObjectPtr gameObject1, GameObjectPtr gameObject2);

class Collider : public Component
{
protected:
	Collider(const Collider &other) = default;
	Collider(ColliderType type);

	// The type of collider used by this component.
	// (Currently, Circle or Line).
	ColliderType type;

	// Pointer to a function that handles collisions between two objects.
	CollisionEventHandler	handler;

	


private:
	static bool IsColliding(ColliderPtr collider1, ColliderPtr collider2);
public:
	virtual ~Collider() = default;
	virtual ColliderPtr Clone(GameObjectPtr parent) = 0;
	virtual ColliderPtr Clone(void) const override;

	virtual void Update(float dt) override;
	virtual void Draw() override;

	void SetCollisionHandler(CollisionEventHandler handler);
	void SetParent(GameObjectPtr parent);

	// Check if two objects are colliding.
	// (Hint: Refer to the project instructions for implementation suggestions.)
	// (Hint: Make sure to call the handler for both colliders, passing the 
	//	  parent game object pointers in the correct order!)
	// Params:
	//	 collider1 = Pointer to the first collider component.
	//	 collider2 = Pointer to the second collider component.
	static void Check(ColliderPtr collider, ColliderPtr other);
};


//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------