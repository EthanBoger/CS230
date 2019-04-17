//------------------------------------------------------------------------------
//
// File Name:	Teleporter.c
// Author(s):	Roland Shum
// Project:		MyGame
// Course:		CS230S19
//
// Copyright © 2019 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Teleporter.h"
#include "Transform.h"
#include "GameObject.h"
#include "Vector2D.h"
#include <AEEngine.h>
//------------------------------------------------------------------------------
// Private Consts:
//------------------------------------------------------------------------------

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

void TeleporterUpdateObject(GameObjectPtr gameObject)
{
	// If gameobject is nothing.
	if (gameObject == NULL)
		return;

	// Get the gameobjects transform
	TransformPtr transform = GameObjectGetTransform(gameObject);
	if (transform == NULL)
		return;

	// Get the gameobjects physics velocity
	PhysicsPtr physics = GameObjectGetPhysics(gameObject);
	if (physics == NULL)
		return;

	// Get the velocity
	Vector2D const *velocity = physics->getVelocity();
	Vector2D const *position = transform->getTranslation();
	Vector2D transport = { 0,0 };

	// Are we moving in the positive x direction?
	if (velocity->x > 0)
	{
		// Check our position against the right screen.
		if (position->x > AEGfxGetWinMaxX())
		{
			transport.x = AEGfxGetWinMinX();
		}
	}
	else // Else we are moving in the negative x direction.
	{
		// Check against left screen.
		if (position->x < AEGfxGetWinMinX())
		{
			transport.x = AEGfxGetWinMaxX();
		}
	}

	// Are we moving in the positive Y direction?
	if (velocity->y > 0)
	{
		// Check position against the bottom
		if (position->y > AEGfxGetWinMaxY())
		{
			transport.y = AEGfxGetWinMinY();
		}
	}
	else // Else we are moving in the negative Y direction
	{
		// Check against the top
		if (position->y < AEGfxGetWinMinY())
		{
			transport.y = AEGfxGetWinMaxY();
		}
	}

	// Do we need to transport the position?
	if (transport.x != 0 || transport.y != 0)
	{
		transform->setTranslation(&transport);
	}

}


//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------