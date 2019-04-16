//------------------------------------------------------------------------------
//
// File Name:	GameObjectManager.h
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

#include "GameObject.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Consts:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

// You are free to change the contents of this structure as long as you do not
//   change the public interface declared in the header.
struct GameObjectManager
{
private:
	static const int objectListSize = 100;
public:
	GameObjectManager();
	~GameObjectManager();

	// The current number of game objects currently in the list.
	unsigned int objectCount;

	// The maximum number of game objects ever in the list, at any given moment.
	unsigned int objectMax;

	// This list can be a fixed-length array (minimum size of 100 entries)
	// or a dynamically sized array, such as a linked list.
	GameObjectPtr objectList[GameObjectManager::objectListSize];


};

class GameObjectManagers
{
private:
	GameObjectManager gameObjectActiveList;
	GameObjectManager gameObjectArchetypes;
public:
	void Update(float dt);
	void CheckCollisions();
	void Draw();
	void Add(GameObjectPtr gameObject);
	void AddArchetype(GameObjectPtr gameObject);
	GameObjectPtr GetObjectByName(const char * name);
	GameObjectPtr GetArchetype(const char * name);

private:
	void AddGameObject(GameObjectPtr *list, GameObjectPtr obj);
	GameObjectPtr FindGameObjectByName(GameObjectPtr *list, const char * name);
};

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------