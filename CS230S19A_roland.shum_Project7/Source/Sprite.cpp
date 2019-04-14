//------------------------------------------------------------------------------
//
// File Name:	Sprite.c
// Author(s):	Roland Shum
// Project:		MyGame
// Course:		CS230S19
//
// Copyright © 2019 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once
#include "stdafx.h"
#include "Sprite.h"
#include "../AE/include/AEEngine.h"
#include "Vector2D.h"
#include "Transform.h"
#include "Trace.h"
#include "SpriteSource.h"
#include "Mesh.h"
#include "Matrix2D.h"
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

// Dynamically allocate a new sprite object.
// (Hint: Use calloc() to ensure that all member variables are initialized to 0.)
// (NOTE: Make sure to initialize the 'alpha' member variable to 1.0f.)
// Params:
//	 name = The name of the sprite.
// Returns:
//	 If the memory allocation was successful,
//	   then return a pointer to the allocated memory,
//	   else return NULL.
Sprite::Sprite(const char * name) : alpha(1.0f), frameIndex(0), mesh(NULL),
	name(name), spriteSource(NULL), text(NULL)
{
}

// Dynamically allocate a clone of an existing sprite.
// (Hint: Perform a shallow copy of the member variables.)
// Params:
//	 other = Pointer to the component to be cloned.
// Returns:
//	 If 'other' is valid and the memory allocation was successful,
//	   then return a pointer to the cloned component,
//	   else return NULL.
Sprite::Sprite(const Sprite& other)
{
  // shallow copy
  *this = other;
}

// Draw a sprite (Sprite can be textured or untextured).
// Params:
//	 sprite = Pointer to the sprite object.
//   transform = Pointer to the sprite object's transform.
void Sprite::Draw(TransformPtr transform)
{
  if (this->mesh == NULL)
    return;

  /* If the this has a source, */
  if (this->spriteSource != NULL)
  {
    /* Set render mode to texture */
    AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
  }
  else
  {
    /* Draw a colored mode if there is no texture */
    AEGfxSetRenderMode(AE_GFX_RM_COLOR);
  }
  /* Set the transparency */
  AEGfxSetTransparency(this->alpha);

  AEGfxSetBlendColor(0.0, 0.0, 0.0, 0.0);

  if (this->text == NULL)
  {
    MeshRender(this->mesh, this->spriteSource, TransformGetMatrix(transform), this->frameIndex);
  }
  else // Else its a text.
  {
    /* Get a local copy of the transformation matrix. */
    Matrix2D transformMtx = *TransformGetMatrix(transform);

    Matrix2D offset;
    Matrix2DTranslate(&offset, TransformGetScale(transform)->x, 0);

    // Local pointer to char's 
    const char * pchar = this->text;

    // Walk through text and render.
    while (*pchar != 0)
    {
      // Convert character to zero-frame index.
      int index = *pchar - ' ';
      MeshRender(this->mesh, this->spriteSource, &transformMtx, index);

      // Move through the string.
      pchar++;

      // Update the position used.
      Matrix2DConcat(&transformMtx, &offset, &transformMtx);
    }
  }
}

// Get a sprite's alpha value.
// Params:
//	 sprite = Pointer to the sprite object.
// Returns:
//	 If the pointer is valid,
//		then return the sprite's alpha value (a value between 0.0f and 1.0f),
//		else return 0.0f.
float Sprite::getAlpha() const
{
    return this->alpha;
}

// Set a sprite's alpha value.
// (NOTE: Make sure to clamp the resulting alpha value between 0.0f and 1.0f, as the
//	input value might be outside of this range.)
// (HINT: See AEClamp in AEMath.h.)
// Params:
//	 sprite = Pointer to the sprite object.
//   newAlpha = The sprite's new alpha value.
void Sprite::setAlpha(float newAlpha)
{
  /* Clamp the alpha value to 0.0 to 1.0 */
  newAlpha = AEClamp(newAlpha, 0.0f, 1.0f);

  /* Set the alpha */
  this->alpha = newAlpha;
}

// Set the sprite's current frame.
// (NOTE: You must verify that the frameIndex parameter is within the
//	range [0 to frame count] before changing the sprite's frame index!)
// Params:
//	 sprite = Pointer to the sprite object.
//   frameIndex = New frame index for the sprite (0 .. frame count).
// ADDITIONAL REQUIREMENTS:
//   This function must make the following function call:
//     TraceMessage("SpriteSetFrame: %s frame index = %d", sprite->name, frameIndex);
void Sprite::setFrame(unsigned frameIndex)
{

  /* Clamp the frameindex */
  frameIndex = (unsigned int)AEClamp((float)frameIndex, 0, (float)SpriteSourceGetFrameCount(this->spriteSource));

  /* Set the frame index. */
  this->frameIndex = frameIndex;

  TraceMessage("SpriteSetFrame: %s frame index = %d", this->name, frameIndex);
}

// Set the sprite's mesh.
// (NOTE: This mesh may be textured or untextured.)
// (NOTE: This mesh may contain any number of triangles.)
// Params:
//	 sprite = Pointer to the sprite object.
//   mesh = Pointer to a mesh created using the Alpha Engine.
void Sprite::setMesh(AEGfxVertexList * mesh)
{
  /* Set the mesh. */
  this->mesh = mesh;
}

// Set a new SpriteSource for the specified sprite.
// (NOTE: The spriteSource parameter may be NULL.  This will remove an existing
//	texture from a sprite and cause the sprite to be displayed as a colored mesh.)
// Params:
//	 sprite = Pointer to the sprite to be modified.
//	 spriteSource = A new sprite source for the sprite (this pointer may be NULL).
void Sprite::setSpriteSource(SpriteSourcePtr spriteSource)
{
  this->spriteSource = spriteSource;
}

// Assign a text string to a sprite object.  This will allow a sequence of
//	 characters to be displayed as text.
// (NOTE: The text parameter may be NULL.  This will remove an existing text
//	 string from a sprite and cause the sprite to be displayed normally.)
// Params:
//	 sprite = Pointer to the sprite to be modified.
//	 text = Pointer to a zero-terminated array of characters.
void Sprite::setText(const char * text)
{
    this->text = text;
}