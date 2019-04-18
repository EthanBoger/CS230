//------------------------------------------------------------------------------
//
// File Name:	GameStateFactory.c
// Author(s):	Roland Shum
// Project:		MyGame
// Course:		CS230S19
//
// Copyright © 2019 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include <AEEngine.h>
#include "GameStateManager.h"
#include "GameObjectFactory.h"
#include "SpriteSource.h"
#include "Trace.h"

#include "BehaviorHudText.h"
#include "BehaviorAsteroid.h"
#include "BehaviorSpaceship.h"
#include "BehaviorBullet.h"

#include "Vector2D.h"
#include "Mesh.h"
#include "SpriteSource.h"
#include "ColliderCircle.h"
#include "Transform.h"
#include "Physics.h"
#include "Sprite.h"
#include "SpriteSource.h"
#include "GameObject.h"
#include "ColliderCircle.h"
#include "ColliderLine.h"
//------------------------------------------------------------------------------
// Private Consts:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------
typedef struct AEGfxVertexList * AEGfxVertexListPtr;
typedef struct AEGfxTexture * AEGfxVertexTexturePtr;
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

// Create a single instance of the specified game object.
// Params:
//	 objectType = The type of game object to be created.
// Returns:
//	 If the objectType is valid (between 0 and cGameObjectTypeCount),
//	   then return a pointer to a new instance of the specified game object type,
//	   else NULL.
GameObjectPtr GameObjectFactory::Create(GameObjectFactory::GameObjectType objectType)
{
	// If not between 0 and cGameObjectTypeCount
	if (!gameObjectTypeIsValid(objectType))
		return NULL;
	switch (objectType)
	{
	case cGameObjectTypeSpaceship:
		return GameObjectFactory::CreateSpaceship();
		break;
	case cGameObjectTypeAsteroid:
		return GameObjectFactory::CreateAsteroid();
		break;
	case cGameObjectTypeBullet:
		return GameObjectFactory::CreateBullet();
		break;
	case cGameObjectTypeHudText:
		return GameObjectFactory::CreateHudText();
		break;
	case cGameObjectTypeArena:
		return GameObjectFactory::CreateArena();
		break;
	default:
		break;
	}
	return NULL;
}

// Free all meshes, textures and sprite sources associated with any created game objects.
void GameObjectFactory::UnloadResources(void)
{
	for (int i = 0; i < cGameObjectTypeCount; i++)
	{
		if (meshList[i] != NULL)
		{
			AEGfxMeshFree(meshList[i]);
			meshList[i] = NULL;
		}
		if (spriteSourceList[i] != NULL)
		{
			delete spriteSourceList[i];
			spriteSourceList[i] = NULL;
		}
		if (textureList[i] != NULL)
		{
			AEGfxTextureUnload(textureList[i]);
			textureList[i] = NULL;
		}
	}
}
//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------
AEGfxVertexListPtr GameObjectFactory::CreateSpaceshipMesh(void)
{
	AEGfxVertexListPtr mesh;
	// Create triangle spaceship
	AEGfxMeshStart();
	AEGfxTriAdd(
		-0.5f, -0.5f, 0xFFFF0000, 0.0f, 0.0f,
		0.5f, 0.0f, 0xFFFFFF00, 0.0f, 0.0f,
		-0.5f, 0.5f, 0xFFFF0000, 0.0f, 0.0f);
	mesh = AEGfxMeshEnd();
	return mesh;
}

AEGfxVertexListPtr GameObjectFactory::CreateBulletMesh(void)
{
	AEGfxVertexListPtr mesh;
	// Create bullet mesh
	AEGfxMeshStart();
	AEGfxTriAdd(
		-0.5f, -0.5f, 0xFFFF0000, 0.0f, 0.0f,
		0.5f, 0.0f, 0xFFFFFF00, 0.0f, 0.0f,
		-0.5f, 0.5f, 0xFFFF0000, 0.0f, 0.0f);
	mesh = AEGfxMeshEnd();
	return mesh;
}

AEGfxVertexListPtr GameObjectFactory::Create16x6Mesh(void)
{
	AEGfxVertexListPtr mesh;
	mesh = MeshCreateQuad(0.5f, 0.5f, 1.0f / 16, 1.0f / 6, "Mesh16x6");
	return mesh;
}

AEGfxVertexListPtr GameObjectFactory::CreateUnitSizedMesh( void)
{
	AEGfxVertexListPtr mesh;
	mesh = MeshCreateQuad(0.5f, 0.5f, 1.0f, 1.0f, "UnitSizeMesh");
	return mesh;
}

AEGfxVertexListPtr GameObjectFactory::CreateAsteroidMesh(void)
{
	AEGfxVertexListPtr mesh;
	AEGfxMeshStart();
	AEGfxTriAdd(
		0.0f, 0.4f, 0xFF808080, 0.0f, 0.0f,
		-0.5f, 0.0f, 0xFFFF0080, 0.0f, 0.0f,
		0.4f, 0.0f, 0xFF808080, 0.0f, 0.0f);
	AEGfxTriAdd(
		-0.5f, 0.0f, 0xFF808080, 0.0f, 0.0f,
		0.0f, -0.3f, 0xFFFF0080, 0.0f, 0.0f,
		0.4f, 0.0f, 0xFF808080, 0.0f, 0.0f);
	AEGfxTriAdd(
		-0.0f, -0.3f, 0xFF808080, 0.0f, 0.0f,
		0.5f, -0.5f, 0xFFFF0080, 0.0f, 0.0f,
		0.4f, 0.0f, 0xFF808080, 0.0f, 0.0f);
	mesh = AEGfxMeshEnd();
	return mesh;
}

AEGfxVertexListPtr GameObjectFactory::CreateMesh(GameObjectType objectType)
{
	switch (objectType)
	{
	case cGameObjectTypeSpaceship:
		return CreateSpaceshipMesh();
		break;
	case cGameObjectTypeAsteroid:
		return CreateAsteroidMesh();
		break;
	case cGameObjectTypeBullet:
		return CreateBulletMesh();
		break;
	case cGameObjectTypeHudText:
		return Create16x6Mesh();
		break;
	case cGameObjectTypeArena:
		return CreateUnitSizedMesh();
		break;
	default:
		break;
	}
	return NULL;
}

AEGfxVertexListPtr GameObjectFactory::GetMesh(GameObjectType objectType)
{
	if (!gameObjectTypeIsValid(objectType))
		return NULL;
	if (meshList[objectType] == NULL)
	{
		meshList[objectType] = GameObjectFactory::CreateMesh(objectType);
	}
	return meshList[objectType];
}

SpriteSourcePtr GameObjectFactory::CreateSpriteSource(GameObjectType objectType)
{
	SpriteSourcePtr result;
	switch (objectType)
	{
	case cGameObjectTypeHudText:
		if ((textureList[objectType] = AEGfxTextureLoad("Assets/FontSheet.png")) == NULL)
		{
			return NULL;
		}
		result = new SpriteSource(16, 6, textureList[objectType]);
		return result ;
		break;
	default:
		break;
	}
	return NULL;
}

SpriteSourcePtr GameObjectFactory::GetSpriteSource(GameObjectType objectType)
{
	if (!gameObjectTypeIsValid(objectType))
		return NULL;
	if (spriteSourceList[objectType] == NULL)
	{
		spriteSourceList[objectType] = GameObjectFactory::CreateSpriteSource(objectType);
	}
	return spriteSourceList[objectType];
}

bool GameObjectFactory::gameObjectTypeIsValid(GameObjectType objectType)
{
	// If not between 0 and cGameObjectTypeCount
	if (!(0 <= objectType && objectType < cGameObjectTypeCount))
		return false;
	return true;
}

GameObjectPtr GameObjectFactory::CreateSpaceship(void)
{
	GameObjectPtr newObj = new GameObject("Spaceship");
	TransformPtr trans = new Transform(0, 0);

	trans->setRotation(0);
	/* Set the scale by making a vector 2d of 100,100 */
	Vector2D vector_scale = { 50, 40 };
	trans->setScale(&vector_scale);

	SpritePtr sprite = new Sprite("Spaceship Sprite");

	sprite->setMesh(GameObjectFactory::GetMesh(cGameObjectTypeSpaceship));

	PhysicsPtr physics = new Physics();
	BehaviorPtr behavior = new Spaceship(newObj);
	ColliderCirclePtr collider = new ColliderCircle();
	collider->SetRadius(trans->getScale()->x / 2.0f);

	/* Set drag */
	physics->setDrag(0.99f);

	newObj->addPhysics(physics);
	newObj->addTransform(trans);
	newObj->addSprite(sprite);
	newObj->addBehavior(behavior);
	newObj->addCollider(collider);

	return newObj;
}

GameObjectPtr GameObjectFactory::CreateAsteroid(void)
{
	GameObjectPtr newObj = new GameObject("Asteroid");
	TransformPtr trans = new Transform(0, 0);

	trans->setRotation(0);
	/* Set the scale by making a vector 2d of 100,100 */
	Vector2D vector_scale = { 40, 40 };
	trans->setScale(&vector_scale);

	SpritePtr sprite = new Sprite("Asteroid Sprite");

	sprite->setMesh(GameObjectFactory::GetMesh(cGameObjectTypeAsteroid));

	PhysicsPtr physics = new Physics();
	physics->setRotationalVelocity(PI / 4.0f);

	BehaviorPtr behavior = new BehaviorAsteroid(newObj);
	ColliderCirclePtr collider = new ColliderCircle();
	collider->SetRadius(trans->getScale->x / 2.0f);

	newObj->addPhysics(physics);
	newObj->addTransform(trans);
	newObj->addSprite(sprite);
	newObj->addBehavior(behavior);
	newObj->addCollider(collider);

	return newObj;
}

GameObjectPtr GameObjectFactory::CreateBullet(void)
{
	GameObjectPtr newObj = new GameObject("Bullet");
	TransformPtr trans = new Transform(0, 0);

	trans->setRotation(0);
	/* Set the scale by making a vector 2d of 100,100 */
	Vector2D vector_scale = { 10,10 };
	trans->setScale(&vector_scale);

	SpritePtr sprite = new Sprite("Bullet Sprite");
	sprite->setMesh(GameObjectFactory::GetMesh(cGameObjectTypeBullet));

	PhysicsPtr physics = new Physics();
	BehaviorPtr behavior = new BehaviorBullet(newObj);
	ColliderCirclePtr collider = new ColliderCircle();
	collider->SetRadius(10.0f);

	newObj->addPhysics(physics);
	newObj->addTransform(trans);
	newObj->addSprite(sprite);
	newObj->addBehavior(behavior);
	newObj->addCollider(collider);

	return newObj;
}

GameObjectPtr GameObjectFactory::CreateHudText(void)
{
	GameObjectPtr newObj = new GameObject("HUD Text");
	TransformPtr trans = new Transform(0, 0);

	trans->setRotation(0);
	/* Set the scale by making a vector 2d of 100,100 */
	Vector2D vector_scale = { 20,30 };
	trans->setScale(&vector_scale);

	SpritePtr sprite = new Sprite("HUD Text Sprite");
	sprite->setMesh(GameObjectFactory::GetMesh(cGameObjectTypeHudText));
	sprite->setSpriteSource(GameObjectFactory::GetSpriteSource(cGameObjectTypeHudText));

	BehaviorPtr behavior = new BehaviorAsteroid(newObj);

	newObj->addTransform(trans);
	newObj->addSprite(sprite);
	newObj->addBehavior(behavior);

	return newObj;
}

GameObjectPtr GameObjectFactory::CreateArena(void)
{
	GameObjectPtr newObj = new GameObject("Arena");
	TransformPtr trans = new Transform(0, 0);

	trans->setRotation(0);
	/* Set the scale by making a vector 2d of 100,100 */
	Vector2D vector_scale = { 400, 200 };
	trans->setScale(&vector_scale);

	SpritePtr sprite = new Sprite("Arena Sprite");
	sprite->setMesh(GameObjectFactory::GetMesh(cGameObjectTypeArena));

	ColliderLinePtr collider = new ColliderLine();

	Vector2D points[4]; // TL, TR, BL, BR
	Vector2DSet(&points[0], AEGfxGetWinMinX(), AEGfxGetWinMaxY());
	Vector2DSet(&points[1], AEGfxGetWinMaxX(), AEGfxGetWinMaxY());
	Vector2DSet(&points[2], AEGfxGetWinMinX(), AEGfxGetWinMinY());
	Vector2DSet(&points[3], AEGfxGetWinMaxX(), AEGfxGetWinMinY());

	collider->AddLineSegment(&points[0], &points[1]);
	collider->AddLineSegment(&points[0], &points[2]);
	collider->AddLineSegment(&points[2], &points[3]);
	collider->AddLineSegment(&points[3], &points[1]);

	// Reuse the four points, this time around the obj : TL, TR, BL, BR
	Vector2DSet(&points[0], -200, 100);
	Vector2DSet(&points[1], 200, 100);
	Vector2DSet(&points[2], -200, -100);
	Vector2DSet(&points[3], 200, -100);


	collider->AddLineSegment(&points[0], &points[1]);
	collider->AddLineSegment(&points[0], &points[2]);
	collider->AddLineSegment(&points[2], &points[3]);
	collider->AddLineSegment(&points[3], &points[1]);

	//ColliderLineAddLineSegment(collider, &points[0], &points[1]); // TL TR
	//ColliderLineAddLineSegment(collider, &points[0], &points[2]); // TL BL
	//ColliderLineAddLineSegment(collider, &points[2], &points[3]); // BL BR
	//ColliderLineAddLineSegment(collider, &points[3], &points[1]); // BR TR

	newObj->addTransform(trans);
	newObj->addSprite(sprite);
	newObj->addCollider(collider);

	return newObj;
}