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
void AnimationAdvanceFrames(AnimationPtr animation, float dt);
//------------------------------------------------------------------------------
// Public Consts:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------


// You are free to change the contents of this structure as long as you do not
//   change the public interface declared in the header.
typedef struct Animation
{
  // Pointer to the sprite associated with the animation.
  SpritePtr sprite;

  // The current frame being displayed.
  unsigned int frameIndex;

  // The maximum number of frames in the sequence.
  unsigned int frameCount;

  // The time remaining for the current frame.
  float frameDelay;

  // The amount of time to display each frame.
  // (Used only when playing simple animations (0 .. numFrame).)
  float frameDuration;

  // True if the animation is running; false if the animation has stopped.
  bool isRunning;

  // True if the animation loops back to the beginning.
  bool isLooping;

  // True if the end of the animation has been reached, false otherwise.
  // (Hint: This should be true for only one game loop.)
  bool isDone;

  // The animation sequence currently being played, if any.
  //AnimationSequencePtr	sequence;

} Animation;


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
AnimationPtr AnimationCreate(SpritePtr sprite)
{
  Animation *newAnim = calloc(1, sizeof(Animation));

  assert(newAnim);

  newAnim->sprite = sprite;

  return newAnim;
}

// Free the memory associated with an animation.
// (Also, set the animation pointer to NULL.)
// Params:
//	 animation = Pointer to the animation pointer.
void AnimationFree(AnimationPtr * animation)
{
  free(*animation);
  *animation = NULL;
}

// Play a simple animation sequence (0 .. frameCount).
// (Hint: Make sure to initialize all variables, except for "sprite".)
// Params:
//	 animation = Pointer to the animation object.
//	 frameCount = The number of frames in the sequence.
//	 frameDuration = The amount of time to wait between frames (in seconds).
//	 isLooping = True if the animation loops, false otherwise.
void AnimationPlay(AnimationPtr animation, int frameCount, float frameDuration, bool isLooping)
{
  if (animation == NULL)
    return;

  animation->frameCount = frameCount;
  animation->frameDuration = frameDuration;
  animation->isLooping = isLooping;
  
  animation->isRunning = true;
  animation->isDone = false;

  SpriteSetFrame(animation->sprite, 0);
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
void AnimationUpdate(AnimationPtr animation, float dt)
{
  if (animation == NULL)
    return;
  animation->isDone = false;

  if (animation->isRunning)
  {
    animation->frameDelay -= dt;
    if (animation->frameDelay <= 0)
    {
      AnimationAdvanceFrames(animation, dt);
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
bool AnimationIsDone(AnimationPtr animation)
{
  if (animation)
  {
    return animation->isDone;
  }
  return false;
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------
void AnimationAdvanceFrames(AnimationPtr animation, float dt)
{
  UNREFERENCED_PARAMETER(dt);

  if (animation == NULL)
    return;

  // Increment frame index
  animation->frameIndex++;
  // If we go above the framecount.
  if (animation->frameIndex >= animation->frameCount)
  {
    // Is this animation supposed to loop?
    if (animation->isLooping)
    {
      animation->frameIndex = 0;
    }
    else
    {
      animation->frameIndex = animation->frameCount - 1;
      animation->isRunning = false;
    }  
    animation->isDone = true;
  }

  if (animation->isRunning)
  {
    // Update Sprite frame
    SpriteSetFrame(animation->sprite, animation->frameIndex);
    animation->frameDelay += animation->frameDuration;
  }
  else
  {
    animation->frameDelay = 0;
  }
}