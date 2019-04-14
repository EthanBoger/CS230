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
static AEGfxVertexListPtr meshList[cGameObjectTypeCount] = { 0 };
static SpriteSourcePtr spriteSourceList[cGameObjectTypeCount] = { 0 };
static AEGfxVertexTexturePtr textureList[cGameObjectTypeCount] = { 0 };


//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------
static bool gameObjectTypeIsValid(GameObjectType objectType);

static AEGfxVertexListPtr GameObjectFactoryGetMesh(GameObjectType objectType);
static AEGfxVertexListPtr GameObjectFactoryCreateMesh(GameObjectType objectType);

static SpriteSourcePtr GameObjectFactoryCreateSpriteSource(GameObjectType objectType);
static SpriteSourcePtr GameObjectFactoryGetSpriteSource(GameObjectType objectType);

static GameObjectPtr GameObjectFactoryCreateSpaceship(void);
static GameObjectPtr GameObjectFactoryCreateAsteroid(void);
static GameObjectPtr GameObjectFactoryCreateBullet(void);
static GameObjectPtr GameObjectFactoryCreateHudText(void);
static GameObjectPtr GameObjectFactoryCreateArena(void);
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
GameObjectPtr GameObjectFactoryCreate(GameObjectType objectType)
{
	// If not between 0 and cGameObjectTypeCount
	if (!gameObjectTypeIsValid(objectType))
		return NULL;
	switch (objectType)
	{
	case cGameObjectTypeSpaceship:
		return GameObjectFactoryCreateSpaceship();
		break;
	case cGameObjectTypeAsteroid:
		return GameObjectFactoryCreateAsteroid();
		break;
	case cGameObjectTypeBullet:
		return GameObjectFactoryCreateBullet();
		break;
	case cGameObjectTypeHudText:
		return GameObjectFactoryCreateHudText();
		break;
	case cGameObjectTypeArena:
		return GameObjectFactoryCreateArena();
		break;
	default:
		break;
	}
	return NULL;
}

// Free all meshes, textures and sprite sources associated with any created game objects.
void GameObjectFactoryUnloadResources(void)
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
			SpriteSourceFree(&spriteSourceList[i]);
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
static AEGfxVertexListPtr CreateSpaceshipMesh(void)
{
	AEGfxVertexListPtr mesh;
	// Create triangle spaceship
	AEGfxMeshStart();
	AEGfxTriAdd(
		-0.5f, -0.5f, 0xFFFF0000, 0.0f, 0.0f,
		0.5f, 0.0f, 0xFFFFFF00, 0.0f, 0.0f,
		-0.5f, 0.5f, 0xFFFF0000, 0.0f, 0.0f);
	mesh = AEGfxMeshEnd();
	AE_ASSERT_MESG(mesh, "Failed to create spaceship mesh!");
	return mesh;
}

static AEGfxVertexListPtr CreateBulletMesh(void)
{
	AEGfxVertexListPtr mesh;
	// Create bullet mesh
	AEGfxMeshStart();
	AEGfxTriAdd(
		-0.5f, -0.5f, 0xFFFF0000, 0.0f, 0.0f,
		0.5f, 0.0f, 0xFFFFFF00, 0.0f, 0.0f,
		-0.5f, 0.5f, 0xFFFF0000, 0.0f, 0.0f);
	mesh = AEGfxMeshEnd();
	AE_ASSERT_MESG(mesh, "Failed to create bullet mesh!");
	return mesh;
}

static AEGfxVertexListPtr Create16x6Mesh(void)
{
	AEGfxVertexListPtr mesh;
	mesh = MeshCreateQuad(0.5f, 0.5f, 1.0f / 16, 1.0f / 6, "Mesh16x6");
	AE_ASSERT_MESG(mesh, "Failed to create 16x6 mesh!");
	return mesh;
}

static AEGfxVertexListPtr CreateUnitSizedMesh( void)
{
	AEGfxVertexListPtr mesh;
	mesh = MeshCreateQuad(0.5f, 0.5f, 1.0f, 1.0f, "UnitSizeMesh");
	AE_ASSERT_MESG(mesh, "Failed to create 16x6 mesh!");
	return mesh;
}

static AEGfxVertexListPtr CreateAsteroidMesh(void)
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
	AE_ASSERT_MESG(mesh, "Failed to create asteroid mesh!");
	return mesh;
}

static AEGfxVertexListPtr GameObjectFactoryCreateMesh(GameObjectType objectType)
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

static AEGfxVertexListPtr GameObjectFactoryGetMesh(GameObjectType objectType)
{
	if (!gameObjectTypeIsValid(objectType))
		return NULL;
	if (meshList[objectType] == NULL)
	{
		meshList[objectType] = GameObjectFactoryCreateMesh(objectType);
	}
	return meshList[objectType];
}

static SpriteSourcePtr GameObjectFactoryCreateSpriteSource(GameObjectType objectType)
{
	SpriteSourcePtr result;
	switch (objectType)
	{
	case cGameObjectTypeHudText:
		if ((textureList[objectType] = AEGfxTextureLoad("Assets/FontSheet.png")) == NULL)
		{
			return NULL;
		}
		if ((result = SpriteSourceCreate(16, 6, textureList[objectType])) == NULL)
		{
			return NULL;
		}
		return result ;
		break;
	default:
		break;
	}
	return NULL;
}

static SpriteSourcePtr GameObjectFactoryGetSpriteSource(GameObjectType objectType)
{
	if (!gameObjectTypeIsValid(objectType))
		return NULL;
	if (spriteSourceList[objectType] == NULL)
	{
		spriteSourceList[objectType] = GameObjectFactoryCreateSpriteSource(objectType);
	}
	return spriteSourceList[objectType];
}

static bool gameObjectTypeIsValid(GameObjectType objectType)
{
	// If not between 0 and cGameObjectTypeCount
	if (!(0 <= objectType && objectType < cGameObjectTypeCount))
		return false;
	return true;
}

static GameObjectPtr GameObjectFactoryCreateSpaceship(void)
{
	GameObjectPtr newObj = GameObjectCreate("Spaceship");
	TransformPtr trans = TransformCreate(0, 0);

	/* Error checking. */
	if (trans == NULL)
		return NULL;

	TransformSetRotation(trans, 0);
	/* Set the scale by making a vector 2d of 100,100 */
	Vector2D vector_scale = { 50, 40 };
	TransformSetScale(trans, &vector_scale);

	SpritePtr sprite = SpriteCreate("Spaceship Sprite");

	/* Error checking */
	if (sprite == NULL)
		return NULL;

	SpriteSetMesh(sprite, GameObjectFactoryGetMesh(cGameObjectTypeSpaceship));

	PhysicsPtr physics = PhysicsCreate();
	BehaviorPtr behavior = BehaviorSpaceshipCreate();
	ColliderPtr collider = ColliderCircleCreate();
	ColliderCircleSetRadius(collider, TransformGetScale(trans)->x / 2.0f);

	/* Set drag */
	PhysicsSetDrag(physics, 0.99f);

	/* Error checking */
	if (physics == NULL)
		return NULL;

	GameObjectAddPhysics(newObj, physics);
	GameObjectAddTransform(newObj, trans);
	GameObjectAddSprite(newObj, sprite);
	GameObjectAddBehavior(newObj, behavior);
	GameObjectAddCollider(newObj, collider);

	return newObj;
}

static GameObjectPtr GameObjectFactoryCreateAsteroid(void)
{
	GameObjectPtr newObj = GameObjectCreate("Asteroid");
	TransformPtr trans = TransformCreate(0, 0);

	/* Error checking. */
	if (trans == NULL)
		return NULL;

	TransformSetRotation(trans, 0);
	/* Set the scale by making a vector 2d of 100,100 */
	Vector2D vector_scale = { 40, 40 };
	TransformSetScale(trans, &vector_scale);

	SpritePtr sprite = SpriteCreate("Asteroid Sprite");

	/* Error checking */
	if (sprite == NULL)
		return NULL;

	SpriteSetMesh(sprite, GameObjectFactoryGetMesh(cGameObjectTypeAsteroid));

	PhysicsPtr physics = PhysicsCreate();
	PhysicsSetRotationalVelocity(physics, PI / 4.0f);

	BehaviorPtr behavior = BehaviorAsteroidCreate();
	ColliderPtr collider = ColliderCircleCreate();
	ColliderCircleSetRadius(collider, TransformGetScale(trans)->x / 2.0f);

	/* Error checking */
	if (physics == NULL || collider == NULL)
		return NULL;

	GameObjectAddPhysics(newObj, physics);
	GameObjectAddTransform(newObj, trans);
	GameObjectAddSprite(newObj, sprite);
	GameObjectAddBehavior(newObj, behavior);
	GameObjectAddCollider(newObj, collider);

	return newObj;
}

static GameObjectPtr GameObjectFactoryCreateBullet(void)
{
	GameObjectPtr newObj = GameObjectCreate("Bullet");
	TransformPtr trans = TransformCreate(0, 0);

	/* Error checking. */
	if (trans == NULL)
		return NULL;

	TransformSetRotation(trans, 0);
	/* Set the scale by making a vector 2d of 100,100 */
	Vector2D vector_scale = { 10,10 };
	TransformSetScale(trans, &vector_scale);

	SpritePtr sprite = SpriteCreate("Bullet Sprite");

	/* Error checking */
	if (sprite == NULL)
		return NULL;

	SpriteSetMesh(sprite, GameObjectFactoryGetMesh(cGameObjectTypeBullet));

	PhysicsPtr physics = PhysicsCreate();
	BehaviorPtr behavior = BehaviorBulletCreate();
	ColliderPtr collider = ColliderCircleCreate();
	ColliderCircleSetRadius(collider, 10.0f);

	/* Error checking */
	if (physics == NULL || behavior == NULL || collider == NULL)
		return NULL;

	GameObjectAddPhysics(newObj, physics);
	GameObjectAddTransform(newObj, trans);
	GameObjectAddSprite(newObj, sprite);
	GameObjectAddBehavior(newObj, behavior);
	GameObjectAddCollider(newObj, collider);

	return newObj;
}

static GameObjectPtr GameObjectFactoryCreateHudText(void)
{
	GameObjectPtr newObj = GameObjectCreate("HUD Text");
	TransformPtr trans = TransformCreate(0, 0);

	/* Error checking. */
	if (trans == NULL)
		return NULL;

	TransformSetRotation(trans, 0);
	/* Set the scale by making a vector 2d of 100,100 */
	Vector2D vector_scale = { 20,30 };
	TransformSetScale(trans, &vector_scale);

	SpritePtr sprite = SpriteCreate("HUD Text Sprite");

	/* Error checking */
	if (sprite == NULL)
		return NULL;

	SpriteSetMesh(sprite, GameObjectFactoryGetMesh(cGameObjectTypeHudText));
	SpriteSetSpriteSource(sprite, GameObjectFactoryGetSpriteSource(cGameObjectTypeHudText));

	BehaviorPtr behavior = BehaviorHudTextCreate();


	GameObjectAddTransform(newObj, trans);
	GameObjectAddSprite(newObj, sprite);
	GameObjectAddBehavior(newObj, behavior);

	return newObj;
}

static GameObjectPtr GameObjectFactoryCreateArena(void)
{
	GameObjectPtr newObj = GameObjectCreate("Arena");
	TransformPtr trans = TransformCreate(0, 0);

	/* Error checking. */
	if (trans == NULL)
		return NULL;

	TransformSetRotation(trans, 0);
	/* Set the scale by making a vector 2d of 100,100 */
	Vector2D vector_scale = { 400, 200 };
	TransformSetScale(trans, &vector_scale);

	SpritePtr sprite = SpriteCreate("Arena Sprite");

	/* Error checking */
	if (sprite == NULL)
		return NULL;

	SpriteSetMesh(sprite, GameObjectFactoryGetMesh(cGameObjectTypeArena));

	ColliderPtr collider = ColliderLineCreate();

	Vector2D points[4]; // TL, TR, BL, BR
	Vector2DSet(&points[0], AEGfxGetWinMinX(), AEGfxGetWinMaxY());
	Vector2DSet(&points[1], AEGfxGetWinMaxX(), AEGfxGetWinMaxY());
	Vector2DSet(&points[2], AEGfxGetWinMinX(), AEGfxGetWinMinY());
	Vector2DSet(&points[3], AEGfxGetWinMaxX(), AEGfxGetWinMinY());

	ColliderLineAddLineSegment(collider, &points[0], &points[1]); // TL TR
	ColliderLineAddLineSegment(collider, &points[0], &points[2]); // TL BL
	ColliderLineAddLineSegment(collider, &points[2], &points[3]); // BL BR
	ColliderLineAddLineSegment(collider, &points[3], &points[1]); // BR TR

	// Reuse the four points, this time around the obj : TL, TR, BL, BR
	Vector2DSet(&points[0], -200, 100);
	Vector2DSet(&points[1], 200, 100);
	Vector2DSet(&points[2], -200, -100);
	Vector2DSet(&points[3], 200, -100);

	ColliderLineAddLineSegment(collider, &points[0], &points[1]); // TL TR
	ColliderLineAddLineSegment(collider, &points[0], &points[2]); // TL BL
	ColliderLineAddLineSegment(collider, &points[2], &points[3]); // BL BR
	ColliderLineAddLineSegment(collider, &points[3], &points[1]); // BR TR

	GameObjectAddTransform(newObj, trans);
	GameObjectAddSprite(newObj, sprite);
	GameObjectAddCollider(newObj, collider);
	return newObj;
}