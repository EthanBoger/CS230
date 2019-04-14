//------------------------------------------------------------------------------
//
// File Name:	Random.c
// Author(s):	Roland Shum
// Project:		MyGame
// Course:		CS230S19
//
// Copyright © 2019 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Random.h"

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

// Initialize the random number generator (RNG).
// You may use the example code from
//	 https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/rand.
// However, if you do so, then you must cite this source within the .c file.
void RandomInit()
{
	// Seed the random generator.
#if _DEBUG
	// Seed it with the same number all the time so its easier to debug.
	srand(0);
#else
	// Seed it so that it uses the time.
	srand((unsigned)time(NULL));
#endif
}

// Generate a random number in the interval [rangeMin, rangeMax] (inclusive).
// You may use the example code from
//	 https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/rand.
// However, if you do so, then you must cite this source within the .c file.
// Params:
//	 rangeMin = The lowest possible number generated.
//	 rangeMax = The highest possible number generated.
// Returns:
//   A random number in the interval [rangeMin, rangeMax].
int RandomRange(int rangeMin, int rangeMax)
{
	return rand() % (rangeMax + 1 - rangeMin) + rangeMin;
}

// Generate a random floating-point number in the interval [rangeMin, rangeMax] (inclusive).
// Params:
//	 rangeMin = The lowest possible number generated.
//	 rangeMax = The highest possible number generated.
float RandomRangeFloat(float rangeMin, float rangeMax)
{
	return  (rangeMax - rangeMin) * ((float)rand() / RAND_MAX)  + rangeMin;
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

