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

// Dynamically allocate a new (Bullet) behavior component.
// (Hint: Use calloc() to ensure that all member variables are initialized to 0.)

BehaviorHudText::BehaviorHudText(GameObjectPtr parent) : Behavior((int)cHudTextInvalid,
	(int)cHudTextIdle, parent), watchValue(NULL)
{

}

BehaviorHudTextPtr BehaviorHudText::Clone(void) const
{
	BehaviorHudTextPtr newAsteroird = new BehaviorHudText(*this);
	return newAsteroird;
}

// Set the display characteristics of the HUD Text object.
// (NOTE: This function must be called after cloning the HUD Text archetype object.)
// Params:
//	 behavior = Pointer to the behavior component.
//	 formatString = Pointer for the string used to format the display text.
//	 watchValue = Pointer to the float value to be displayed.
void BehaviorHudText::SetDisplay(const char * formatString_l, const int * watchValue_l)
{
	this->formatString = formatString_l;
	this->watchValue = watchValue_l;
}

// Initialize the current state of the behavior component.
// (Hint: Refer to the lecture notes on finite state machines (FSM).)
// Params:
//	 behavior = Pointer to the behavior component.
void BehaviorHudText::OnInit()
{
	SpritePtr sprite = this->parent->getSprite();

	if (sprite != NULL)
	{
		sprite->setText(this->displayString);
	}
	UpdateText();
}

// Update the current state of the behavior component.
// (Hint: Refer to the lecture notes on finite state machines (FSM).)
// Params:
//	 behavior = Pointer to the behavior component.
//	 dt = Change in time (in seconds) since the last game loop.
void BehaviorHudText::OnUpdate(float dt)
{
	UNREFERENCED_PARAMETER(dt);
	if (this->watchValue != NULL)
	{
		UpdateText();
	}
}

// Exit the current state of the behavior component.
// (Hint: Refer to the lecture notes on finite state machines (FSM).)
// Params:
//	 behavior = Pointer to the behavior component.
//	 dt = Change in time (in seconds) since the last game loop.
void BehaviorHudText::OnExit()
{
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

void BehaviorHudText::UpdateText()
{
	if (this->watchValue != NULL)
	{
		this->displayValue = *this->watchValue;
		sprintf_s(this->displayString, 32, this->formatString, this->displayValue);
	}
}