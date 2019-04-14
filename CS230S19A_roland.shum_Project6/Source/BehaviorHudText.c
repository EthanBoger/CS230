//------------------------------------------------------------------------------
//
// File Name:	BehaviorHudText.c
// Author(s):	Roland Shum
// Project:		MyGame
// Course:		CS230S19
//
// Copyright © 2019 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "BehaviorHudText.h"
#include "Behavior.h"
#include "GameObject.h"
#include "Sprite.h"
//------------------------------------------------------------------------------
// Private Consts:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------
typedef enum HudTextStates
{
	cHudTextInvalid = -1,	// HUD Text has not yet been initialized.
	cHudTextIdle,			// HUD Text will normally remain static.

} HudTextStates;

typedef struct BehaviorHudText
{
	// Inherit the base behavior structure.
	Behavior	base;

	// Add HUD Text-specific behavior variables.

	// The format string to be used with sprintf_s() when updating the HUD Text object.
	// (For example: "Score: %d")
	const char * formatString;

	// The buffer to be used with sprintf_s() when updating the HUD Text object.
	// (For example: "Score: 9001")
	// (NOTE: This buffer has an arbitrary length of 32 chars, which is sufficient for this project.
	//	Exercise caution when using buffers of a fixed length (e.g. use sprintf_s).
	char displayString[32];

	// Pointer to the int value to be used when displaying the HUD Text object.
	// (NOTE: This value will be compared against displayValue to determine when the text must be updated.)
	const int * watchValue;

	// The value currently being displayed by the HUD Text object.
	// (NOTE: This value can be compared with *watchValue to determine when the text must be updated.)
	// (NOTE: Make sure to update this value each time the text is updated.)
	int displayValue;

} BehaviorHudText, *BehaviorHudTextPtr;
//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------
static void BehaviorHudTextUpdateText(BehaviorHudTextPtr);
//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Dynamically allocate a new (Bullet) behavior component.
// (Hint: Use calloc() to ensure that all member variables are initialized to 0.)
BehaviorPtr BehaviorHudTextCreate(void)
{
	BehaviorHudTextPtr newBehavior = calloc(1, sizeof(BehaviorHudText));
	if (newBehavior == NULL)
		return NULL;
	newBehavior->base.memorySize = sizeof(BehaviorHudText);
	newBehavior->base.onInit = BehaviorHudTextInit;
	newBehavior->base.onUpdate = BehaviorHudTextUpdate;
	newBehavior->base.onExit = BehaviorHudTextExit;
	newBehavior->base.stateCurr = cHudTextInvalid;
	newBehavior->base.stateNext = cHudTextIdle;

	return (BehaviorPtr)newBehavior;
}

// Set the display characteristics of the HUD Text object.
// (NOTE: This function must be called after cloning the HUD Text archetype object.)
// Params:
//	 behavior = Pointer to the behavior component.
//	 formatString = Pointer for the string used to format the display text.
//	 watchValue = Pointer to the float value to be displayed.
void BehaviorHudTextSetDisplay(BehaviorPtr behavior, const char * formatString, const int * watchValue)
{
	BehaviorHudTextPtr hud = (BehaviorHudTextPtr)behavior;
	hud->formatString = formatString;
	hud->watchValue = watchValue;
}

// Initialize the current state of the behavior component.
// (Hint: Refer to the lecture notes on finite state machines (FSM).)
// Params:
//	 behavior = Pointer to the behavior component.
void BehaviorHudTextInit(BehaviorPtr behavior)
{
	BehaviorHudTextPtr hud = (BehaviorHudTextPtr)behavior;
	SpritePtr sprite = GameObjectGetSprite(behavior->parent);

	if (sprite != NULL)
	{
		SpriteSetText(sprite, hud->displayString);
	}
	BehaviorHudTextUpdateText(hud);
}

// Update the current state of the behavior component.
// (Hint: Refer to the lecture notes on finite state machines (FSM).)
// Params:
//	 behavior = Pointer to the behavior component.
//	 dt = Change in time (in seconds) since the last game loop.
void BehaviorHudTextUpdate(BehaviorPtr behavior, float dt)
{
	UNREFERENCED_PARAMETER(dt);
	BehaviorHudTextPtr hud = (BehaviorHudTextPtr)behavior;
	if (hud->watchValue != NULL)
	{
		BehaviorHudTextUpdateText(hud);
	}
}

// Exit the current state of the behavior component.
// (Hint: Refer to the lecture notes on finite state machines (FSM).)
// Params:
//	 behavior = Pointer to the behavior component.
//	 dt = Change in time (in seconds) since the last game loop.
void BehaviorHudTextExit(BehaviorPtr behavior)
{
	UNREFERENCED_PARAMETER(behavior);
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

static void BehaviorHudTextUpdateText(BehaviorHudTextPtr hud)
{
	if (hud->watchValue != NULL)
	{
		hud->displayValue = *hud->watchValue;
		sprintf_s(hud->displayString, 32, hud->formatString, hud->displayValue);
	}
}