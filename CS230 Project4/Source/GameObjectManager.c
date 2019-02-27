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
#define objectListSize 100
//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------
// An example of the structure to be defined in GameStateManager.c.
// You are free to change the contents of this structure as long as you do not
//   change the public interface declared in the header.
typedef struct GameObjectManager
{
  // The current number of game objects currently in the list.
  unsigned int objectCount;

  // The maximum number of game objects ever in the list, at any given moment.
  unsigned int objectMax;

  // This list can be a fixed-length array (minimum size of 100 entries)
  // or a dynamically sized array, such as a linked list.
  GameObjectPtr	objectList[objectListSize];

} GameObjectManager;
//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------
static GameObjectManager gameObjectActiveList;
static GameObjectManager gameObjectArchetypes;
//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------
static void AddGameObject(GameObjectPtr *list, GameObjectPtr obj);
static GameObjectPtr FindGameObjectByName(GameObjectPtr *list, const char * name);
//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Initialize the game object manager.
// (NOTE: This function should initialize the lists for both active game objects
//    and the archetype objects.)
void GameObjectManagerInit(void)
{
  gameObjectActiveList.objectMax = objectListSize;
  gameObjectArchetypes.objectMax = objectListSize;
  gameObjectActiveList.objectCount = 0;
  gameObjectArchetypes.objectCount = 0;
}

// Update all objects in the active game objects list.
// (NOTE: After a game object has been updated, if it has been flagged as
//    destroyed, then it must be removed from the list and freed properly.
//    Additionally, the count of active objects must be reduced by 1.)
// Params:
//	 dt = Change in time (in seconds) since the last game loop.
void GameObjectManagerUpdate(float dt)
{
  for (int i = 0; i < objectListSize; i++)
  {
    // If not null
    if (gameObjectActiveList.objectList[i] != NULL)
    {
      // update it
      GameObjectUpdate(gameObjectActiveList.objectList[i], dt);
    }
  }

  // Destroy any marked GOs
  for (int i = 0; i < objectListSize; i++)
  {
    // If not null
    if (gameObjectActiveList.objectList[i] != NULL)
    {
      // If marked for destruction
      if (GameObjectIsDestroyed(gameObjectActiveList.objectList[i]))
      {
        // Free and set to NULL.
        GameObjectFree(&gameObjectActiveList.objectList[i]);
      }
    }
  }
}

// Draw all game objects in the active game object list.
void GameObjectManagerDraw(void)
{
  for (int i = 0; i < objectListSize; i++)
  {
    // If not null
    if (gameObjectActiveList.objectList[i] != NULL)
    {
      // Draw it
      GameObjectDraw(gameObjectActiveList.objectList[i]);
    }
  }
}

// Shutdown the game object manager.
// (NOTE: This means removing all game objects from both the active and
//	  archetype game object lists.  Make sure that the object counts are
//	  properly updated in both situations.)
void GameObjectManagerShutdown(void)
{
  for (int i = 0; i < objectListSize; i++)
  {
    // If not null
    if (gameObjectActiveList.objectList[i] != NULL)
    {
      // Destroy from active list
      GameObjectFree(&gameObjectActiveList.objectList[i]);
    } 
    if (gameObjectArchetypes.objectList[i] != NULL)
    {
      // Destroy from archetype list
      GameObjectFree(&gameObjectArchetypes.objectList[i]);
    }
  }
}

// Add a game object to the active game object list.
// (Hint: This function and the GameObjectManagerAddArchetype functions require
//    the same code.  Consider creating a single function that adds an object
//	  to a list that is passed as a parameter.)
// Params:
//	 gameObject = Pointer to the game object to be added to the list.
void GameObjectManagerAdd(GameObjectPtr gameObject)
{
  AddGameObject(gameObjectActiveList.objectList, gameObject);
}

// Add a game object to the game object archetype list.
// (Hint: This function and the GameObjectManagerAdd functions require
//    the same code.  Consider creating a single, private function that adds
//	 an object to a list that is passed as a parameter.)
// Params:
//	 gameObject = Pointer to the game object to be added to the list.
void GameObjectManagerAddArchetype(GameObjectPtr gameObject)
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
GameObjectPtr GameObjectManagerGetObjectByName(const char * name)
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
GameObjectPtr GameObjectManagerGetArchetype(const char * name)
{
  return FindGameObjectByName(gameObjectArchetypes.objectList, name);
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------
static void AddGameObject(GameObjectPtr *list, GameObjectPtr obj)
{
  for (int i = 0; i < objectListSize; i++)
  {
    if (list[i] == NULL)
    {
      list[i] = obj;
      return;
    }
  }
}

static GameObjectPtr FindGameObjectByName(GameObjectPtr *list, const char * name)
{
  for (int i = 0; i < objectListSize; i++)
  {
    // If the slot is not empty
    if (list[i] != NULL)
    {
      // If it has the same name.
      if (strcmp(GameObjectGetName(list[i]), name) == 0)
      {
        // Return the ptr;
        return list[i];
      }
    }
  }
  return NULL;
}