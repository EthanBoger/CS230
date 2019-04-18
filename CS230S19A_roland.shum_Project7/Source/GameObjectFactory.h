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

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

typedef struct GameObject *GameObjectPtr;
typedef struct AEGfxVertexList * AEGfxVertexListPtr;
typedef struct AEGfxTexture * AEGfxVertexTexturePtr;
typedef struct SpriteSource* SpriteSourcePtr;
//------------------------------------------------------------------------------
// Public Consts:
//------------------------------------------------------------------------------


class GameObjectFactory
{
public:
	enum GameObjectType
	{
		cGameObjectTypeSpaceship,
		cGameObjectTypeAsteroid,
		cGameObjectTypeBullet,
		cGameObjectTypeHudText,
		cGameObjectTypeArena,

		cGameObjectTypeCount,

	};
	static GameObjectPtr Create(GameObjectType objectType);
	static void UnloadResources();

private:

	static AEGfxVertexListPtr meshList[GameObjectFactory::GameObjectType::cGameObjectTypeCount];
	static SpriteSourcePtr spriteSourceList[GameObjectFactory::GameObjectType::cGameObjectTypeCount];
	static AEGfxVertexTexturePtr textureList[GameObjectFactory::GameObjectType::cGameObjectTypeCount];

	static bool gameObjectTypeIsValid(GameObjectType objectType);

	static AEGfxVertexListPtr GetMesh(GameObjectType objectType);
	static AEGfxVertexListPtr CreateMesh(GameObjectType objectType);

	static SpriteSourcePtr CreateSpriteSource(GameObjectType objectType);
	static SpriteSourcePtr GetSpriteSource(GameObjectType objectType);

	static GameObjectPtr CreateSpaceship(void);
	static GameObjectPtr CreateAsteroid(void);
	static GameObjectPtr CreateBullet(void);
	static GameObjectPtr CreateHudText(void);
	static GameObjectPtr CreateArena(void);

	static AEGfxVertexListPtr CreateBulletMesh(void);
	static AEGfxVertexListPtr CreateSpaceshipMesh(void);
	static AEGfxVertexListPtr Create16x6Mesh(void);
	static AEGfxVertexListPtr CreateUnitSizedMesh(void);
	static AEGfxVertexListPtr CreateAsteroidMesh(void);

};

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
