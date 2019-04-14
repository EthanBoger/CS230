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


// You are free to change the contents of this structure as long as you do not
//   change the public interface declared in the header.
struct Animation
{
private:
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
	
public:
	Animation(SpritePtr sprite);
	Animation(AnimationPtr orig, SpritePtr sprite);
	~Animation();

	bool IsDone() const;

	void Update(float dt);
	void Play(int frameCount, float frameDuration, bool isLooping);
private:
	void advanceFrames(float dt);
};

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------