//------------------------------------------------------------------------------
//
// File Name:	Animation.h
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
#include "stdafx.h"
#include "Animation.h"
#include <assert.h>
#include "Sprite.h"
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

typedef struct Animation * AnimationPtr;
typedef struct Sprite * SpritePtr;
//------------------------------------------------------------------------------
// Public Consts:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Dynamically allocate a new animation object.
// (Hint: Use calloc() to ensure that all member variables are initialized to 0.)
// Params:
//	 sprite = Pointer to the sprite object associated with the animation.
Animation::Animation(SpritePtr sprite) : sprite(sprite)
{

}

// Dynamically allocate a clone of an existing animation.
// (Hint: Perform a shallow copy of the member variables, then change the 'sprite' pointer.)
// Params:
//	 other = Pointer to the component to be cloned.
// Returns:
//	 If 'other' is valid and the memory allocation was successful,
//	   then return a pointer to the cloned component,
//	   else return NULL.
Animation::Animation(AnimationPtr orig, SpritePtr sprite)
{
  // Shallow copy
  *this = *orig;

  // Point to new sprite.
  this->sprite = sprite;
}

// Free the memory associated with an animation.
// (Also, set the animation pointer to NULL.)
// Params:
//	 animation = Pointer to the animation pointer.
Animation::~Animation()
{
}

// Play a simple animation sequence (0 .. frameCount).
// (Hint: Make sure to initialize all variables, except for "sprite".)
// Params:
//	 animation = Pointer to the animation object.
//	 frameCount = The number of frames in the sequence.
//	 frameDuration = The amount of time to wait between frames (in seconds).
//	 isLooping = True if the animation loops, false otherwise.
void Animation::Play(int frameCount, float frameDuration, bool isLooping)
{
  this->frameCount = frameCount;
  this->frameDuration = frameDuration;
  this->isLooping = isLooping;
  
  this->isRunning = true;
  this->isDone = false;

  sprite->setFrame(0);
}

// Play a complex animation sequence (any frame/any delay).
// (Hint: Make sure to initialize all variables, except for "sprite".)
// Params:
//	 animation = Pointer to the animation object.
//	 sequence = Pointer to the animation sequence to be played.
//void AnimationPlaySequence(AnimationPtr animation, AnimationSequencePtr sequence);

// Update the animation.
// Params:
//	 animation = Pointer to the animation object.
//	 dt = Change in time (in seconds) since the last game loop.
void Animation::Update(float dt)
{
	this->isDone = false;

  if (this->isRunning)
  {
	  this->frameDelay -= dt;
    if (this->frameDelay <= 0)
    {
      advanceFrames(dt);
    }
  }
}

// Determine if the animation has reached the end of its sequence.
// Params:
//	 animation = Pointer to the animation object.
// Returns:
//	 If the animation pointer is valid,
//		then return the value in isDone,
//		else return false.
bool Animation::IsDone() const
{
	return this->isDone;
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------
void Animation::advanceFrames(float dt)
{
  UNREFERENCED_PARAMETER(dt);

  // Increment frame index
  this->frameIndex++;
  // If we go above the framecount.
  if (this->frameIndex >= this->frameCount)
  {
    // Is this this supposed to loop?
    if (this->isLooping)
    {
      this->frameIndex = 0;
    }
    else
    {
      this->frameIndex = this->frameCount - 1;
      this->isRunning = false;
    }  
    this->isDone = true;
  }

  if (this->isRunning)
  {
    // Update Sprite frame
    sprite->setFrame(this->frameIndex);
    this->frameDelay += this->frameDuration;
  }
  else
  {
    this->frameDelay = 0;
  }
}
