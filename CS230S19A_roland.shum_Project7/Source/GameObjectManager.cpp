//------------------------------------------------------------------------------
//
// File Name:	GameObjectManager.c
// Author(s):	Roland Shum
// Project:		MyGame
// Course:		CS230S19
//
// Copyright © 2019 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "GameObjectManager.h"

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

GameObjectManager::GameObjectManager() : objectMax(objectListSize), objectCount(0)
{

}

GameObjectManager::~GameObjectManager()
{
	for (int i = 0; i < objectMax; i++)
	{
		// If not null
		if (objectList[i] != NULL)
		{
			// Destroy from active list
			delete objectList[i];
			objectList[i] = NULL;
		}
	}
}

GameObjectManagers::~GameObjectManagers()
{
	UnloadResource();
}

// Update all objects in the active game objects list.
// (NOTE: After a game object has been updated, if it has been flagged as
//    destroyed, then it must be removed from the list and freed properly.
//    Additionally, the count of active objects must be reduced by 1.)
// Params:
//	 dt = Change in time (in seconds) since the last game loop.
void GameObjectManagers::Update(float dt)
{
  for (int i = 0; i < gameObjectActiveList.objectMax; i++)
  {
    // If not null
    if (gameObjectActiveList.objectList[i] != NULL)
    {
      // update it
		gameObjectActiveList.objectList[i]->Update(dt);
    }
  }

  // Destroy any marked GOs
  for (int i = 0; i < gameObjectActiveList.objectMax; i++)
  {
    // If not null
    if (gameObjectActiveList.objectList[i] != NULL)
    {
      // If marked for destruction
      if (gameObjectActiveList.objectList[i]->isDestroyed())
      {
        // Free and set to NULL.
		  delete gameObjectActiveList.objectList[i];
		  gameObjectActiveList.objectList[i] = NULL;
      }
    }
  }
}

// Draw all game objects in the active game object list.
void GameObjectManagers::Draw(void)
{
  for (int i = 0; i < gameObjectActiveList.objectMax; i++)
  {
    // If not null
    if (gameObjectActiveList.objectList[i] != NULL)
    {
      // Draw it
		gameObjectActiveList.objectList[i]->Draw();
    }
  }
}

// Add a game object to the active game object list.
// (Hint: This function and the GameObjectManagerAddArchetype functions require
//    the same code.  Consider creating a single function that adds an object
//	  to a list that is passed as a parameter.)
// Params:
//	 gameObject = Pointer to the game object to be added to the list.
void GameObjectManagers::Add(GameObjectPtr gameObject)
{
  AddGameObject(gameObjectActiveList.objectList, gameObject);
}

// Add a game object to the game object archetype list.
// (Hint: This function and the GameObjectManagerAdd functions require
//    the same code.  Consider creating a single, private function that adds
//	 an object to a list that is passed as a parameter.)
// Params:
//	 gameObject = Pointer to the game object to be added to the list.
void GameObjectManagers::AddArchetype(GameObjectPtr gameObject)
{
  AddGameObject(gameObjectArchetypes.objectList, gameObject);
}

// Returns a pointer to the first active game object matching the specified name.
// (Hint: This function and the GameObjectManagerGetObjectByName functions require
//    the same code.  Consider creating a single, private function that searches
//	  for the named game object in a list that is passed as a parameter.)
// Params:
//	 name = The name of the archetype to be returned, if found.
// Returns:
//   If the named archetype is found,
//	   then return the pointer to the named game object archetype,
//	   else return NULL.
GameObjectPtr GameObjectManagers::GetObjectByName(const char * name)
{
  return FindGameObjectByName(gameObjectActiveList.objectList, name);
}

// Returns a pointer to the first game object archetype matching the specified name.
// (Hint: This function and the GameObjectManagerGetObjectByName functions require
//    the same code.  Consider creating a single, private function that searches
//	  for the named game object in a list that is passed as a parameter.)
// Params:
//	 name = The name of the archetype to be returned, if found.
// Returns:
//   If the named archetype is found,
//	   then return the pointer to the named game object archetype,
//	   else return NULL.
GameObjectPtr GameObjectManagers::GetArchetype(const char * name)
{
  return FindGameObjectByName(gameObjectArchetypes.objectList, name);
}

// Check collisions between all objects held by the game object manager.
void GameObjectManagers::CheckCollisions(void)
{
	for (int i = 0; i < gameObjectActiveList.objectMax; i++)
	{
		if (gameObjectActiveList.objectList[i] != NULL)
		{
			ColliderPtr collider1 = gameObjectActiveList.objectList[i]->getCollider();
			if (collider1 != NULL)
			{
				for (int j = 0; j < i; j++)
				{
					if (gameObjectActiveList.objectList[j] != NULL)
					{
						ColliderPtr collider2 = gameObjectActiveList.objectList[j]->getCollider();
						if (collider2 != NULL)
						{
							// Check fo collision.
							Collider::Check(collider1, collider2);
						}
					}
				}
			}
		}
	}
}

void GameObjectManagers::UnloadResource()
{
	// Destroy any marked GOs
	for (int i = 0; i < gameObjectActiveList.objectMax; i++)
	{
		// If not null
		if (gameObjectActiveList.objectList[i] != NULL)
		{
			// If marked for destruction
			if (gameObjectActiveList.objectList[i]->isDestroyed())
			{
				// Free and set to NULL.
				delete gameObjectActiveList.objectList[i];
				gameObjectActiveList.objectList[i] = NULL;
			}
		}
		if (gameObjectArchetypes.objectList[i] != NULL)
		{
			// If marked for destruction
			if (gameObjectArchetypes.objectList[i]->isDestroyed())
			{
				// Free and set to NULL.
				delete gameObjectArchetypes.objectList[i];
				gameObjectArchetypes.objectList[i] = NULL;
			}
		}
	}
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------
void GameObjectManagers::AddGameObject(GameObjectPtr *list, GameObjectPtr obj)
{
  for (int i = 0; i < gameObjectActiveList.objectMax; i++)
  {
    if (list[i] == NULL)
    {
      list[i] = obj;
      return;
    }
  }
}

GameObjectPtr GameObjectManagers::FindGameObjectByName(GameObjectPtr *list, const char * name)
{
  for (int i = 0; i < gameObjectActiveList.objectMax; i++)
  {
    // If the slot is not empty
    if (list[i] != NULL)
    {
      // If it has the same name.
      if (strcmp(list[i]->getName(), name) == 0)
      {
        // Return the ptr;
        return list[i];
      }
    }
  }
  return NULL;
}