//------------------------------------------------------------------------------
//
// File Name:	Behavior.c
// Author(s):	Roland Shum
// Project:		MyGame
// Course:		CS230S19
//
// Copyright © 2019 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Behavior.h"

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
//
Behavior::Behavior(int curr, int next, GameObjectPtr parent) : stateCurr(curr), stateNext(next), 
	Component(TypeEnum::Behavior, parent)
{
}


// Free the memory associated with a behavior component.
// (Also, set the behavior pointer to NULL.)
// Params:
//	 behavior = Pointer to the behavior component.
Behavior::~Behavior()
{
	//this->Exit();
}

// Update the behavior component.
// (Hint: Refer to the Word document for detailed instructions regarding this function.)
// Params:
//	 behavior = Pointer to the behavior component.
//	 dt = Change in time (in seconds) since the last game loop.
void Behavior::Update(float dt)
{
  // We are changing states if this is true.
  if (this->stateCurr != this->stateNext)
  {
    /* Exit the this if the function exists. */
    this->OnExit();

    // Set the current state.
    this->stateCurr = this->stateNext;

    // Run the init this if it has one.
   this->OnInit();
  }

  // Run the update function if it has one
  this->OnUpdate(dt);

}

void Behavior::Draw()
{

}
//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

