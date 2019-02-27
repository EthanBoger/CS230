//------------------------------------------------------------------------------
//
// File Name:	Mesh.c
// Author(s):	Roland Shum
// Project:		MyGame
// Course:		CS230S19
//
// Copyright © 2019 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------
#include "stdafx.h"
#include "Mesh.h"
#include "../AE/include/AEEngine.h"
#include "SpriteSource.h"
#include "Transform.h"
//------------------------------------------------------------------------------
// Include Files:
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

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Create a quadrilateral mesh using the Alpha Engine.
// Params:
//	 xHalfSize = The X half-size of the mesh.
//	 yHalfSize = The Y half-size of the mesh.
//   uSize = The U size of the mesh, relative to texture coordinates (0.0 .. 1.0).
//   vSize = The V size of the mesh, relative to texture coordinates (0.0 .. 1.0).
//	 name = A name for the mesh.  Currently used only for error messaging.
// Returns:
//	 If the mesh was created successfully,
//	   then return a pointer to the created mesh,
//	   else return NULL.
AEGfxVertexList * MeshCreateQuad(float xHalfSize, float yHalfSize, float uSize, float vSize, const char * name)
{
  AEGfxVertexList *result;

  AEGfxMeshStart();
  AEGfxTriAdd(
    -xHalfSize, -yHalfSize, 0xFFFFFFFF, 0.0f, vSize,
    xHalfSize, -yHalfSize, 0xFFFFFFFF, uSize, vSize,
    -xHalfSize, yHalfSize, 0xFFFFFFFF, 0.0f, 0.0f);

  AEGfxTriAdd(
    xHalfSize, -yHalfSize, 0xFFFFFFFF, uSize, vSize,
    xHalfSize, yHalfSize, 0xFFFFFFFF, uSize, 0.0f,
    -xHalfSize, yHalfSize, 0xFFFFFFFF, 0.0f, 0.0f);

  result = AEGfxMeshEnd();
  /* Error checking */
  AE_ASSERT_MESG(result, "Failed to create mesh %s!", name);

  return result;
}

void MeshRender(AEGfxVertexList * mesh, SpriteSource * spriteSource, Matrix2D * transform, unsigned frameIndex)
{
  assert(mesh);
  assert(transform);

  if (spriteSource != NULL)
  {
    /* Find the u and v texture to use from spritesource. */
    float u, v;
    SpriteSourceGetUV(spriteSource, frameIndex, &u, &v);

    /* Set the texture. */
    AEGfxTextureSet(SpriteSourceGetTexture(spriteSource), u, v);
  }

  /* Set the scale, translation, and rotation in the world. */
  AEGfxSetTransform((void*)transform);

  AEGfxMeshDraw(mesh, AE_GFX_MDM_TRIANGLES);
}