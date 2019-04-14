//------------------------------------------------------------------------------
//
// File Name:	GameObjectFactory.h
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

//------------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
	/* Assume C declarations for C++ */
#endif

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

typedef struct GameObject *GameObjectPtr;

//------------------------------------------------------------------------------
// Public Consts:
//------------------------------------------------------------------------------

typedef enum GameObjectType
{
	cGameObjectTypeSpaceship,
	cGameObjectTypeAsteroid,
	cGameObjectTypeBullet,
	cGameObjectTypeHudText,
	cGameObjectTypeArena,

	cGameObjectTypeCount,

} GameObjectType;

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Create a single instance of the specified game object.
// Params:
//	 objectType = The type of game object to be created.
// Returns:
//	 If the objectType is valid (between 0 and cGameObjectTypeCount),
//	   then return a pointer to a new instance of the specified game object type,
//	   else NULL.
GameObjectPtr GameObjectFactoryCreate(GameObjectType objectType);

// Free all meshes, textures and sprite sources associated with any created game objects.
void GameObjectFactoryUnloadResources(void);

//------------------------------------------------------------------------------

#ifdef __cplusplus
}                       /* End of extern "C" { */
#endif

