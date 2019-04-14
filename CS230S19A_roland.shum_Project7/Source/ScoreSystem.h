//------------------------------------------------------------------------------
//
// File Name:	ScoreSystem.h
// Author(s):	Doug Schilling (dschilling)
// Project:		Project 6
// Course:		CS230S19
//
// Copyright © 2019 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------
//#include "Vector2D.h"
//------------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
	/* Assume C declarations for C++ */
#endif

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------
	typedef struct Vector2D Vector2D;
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

// Reset the Score system.
// (NOTE: All private variables must be initialized to 0.)
void ScoreSystemResetScores();

// Restart the Score system after updating the high score.
// (HINT: The high score value should be updated before zeroing out the score and wave count variables.
void ScoreSystemRestart();

// Increase the game score by score value.
// Params:
//	 scoreValue = The amount to be added to the game score.
void ScoreSystemIncreaseScore(unsigned int scoreValue);

// Increase the wave count by 1.
void ScoreSystemIncreaseWaveCount();

// Create HUD Text objects for "High Score", "Score", and "Wave Count".
// (NOTE: The positions variable must point at an array of three Vector2D structures.)
// Params:
//	 positions = Vector2D array of positions for the HUD Text objects.
void ScoreSystemCreateHudElements(const Vector2D * positions);

//------------------------------------------------------------------------------

#ifdef __cplusplus
}                       /* End of extern "C" { */
#endif

