//------------------------------------------------------------------------------
//
// File Name:	Transform.c
// Author(s): Roland Shum
// Project:		MyGame
// Course:		CS230S19
//
// Copyright © 2019 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once
#include "stdafx.h"
#include "Transform.h"
#include "Vector2D.h"
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

typedef struct Transform * TransformPtr;

// An example of the structure to be defined in Transform.c.

// You are free to change the contents of this structure as long as you do not
//   change the public interface declared in the header.
typedef struct Transform
{
	// The translation (or world position) of a game object.
	Vector2D	translation;

	// The rotation (or orientation) of a game object. (Radians)
	float	rotation;

	// The scale (or size) of a game object.
	// (Hint: this should be initialized to (1, 1).)
	Vector2D	scale;

  // The transformation matrix resulting from concatenating the 
  //	 matrix = Translation*Rotation*Scale matrices.
  Matrix2D	matrix;

  // True if the transformation matrix needs to be recalculated.
  // (Hint: this should be initialized to true.)
  bool	isDirty;

} Transform;

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Dynamically allocate a new transform object.
// (Hint: Use calloc() to ensure that all member variables are initialized to 0.)
// (Hint: Failing to initialize the scale values to non-zero values will result in invisible sprites.)
// Params:
//	 x = Initial world position on the x-axis.
//	 y = Initial world position on the y-axis.
// Returns:
//	 If the memory allocation was successful,
//	   then return a pointer to the allocated memory,
//	   else return NULL.
TransformPtr TransformCreate(float x, float y)
{
  /* Malloc new transform. */
  TransformPtr newTransform = malloc(sizeof(Transform));

  /* Check for failure. */
  if (newTransform == NULL)
  {
    return NULL;
  }

  /* Set values */
  Vector2DSet(&newTransform->translation, x, y);
  Vector2DSet(&newTransform->scale, 1, 1);

  newTransform->rotation = 0;
  newTransform->isDirty = true;
  Matrix2DIdentity(&newTransform->matrix);

  return newTransform;
}

// Dynamically allocate a clone of an existing transform.
// (Hint: Make sure to perform a shallow copy or deep copy, as appropriate.)
// Params:
//	 other = Pointer to the component to be cloned.
// Returns:
//	 If 'other' is valid and the memory allocation was successful,
//	   then return a pointer to the cloned component,
//	   else return NULL.
TransformPtr TransformClone(const TransformPtr other)
{
  if (other == NULL)
    return NULL;

  /* Malloc new transform. */
  TransformPtr newTransform = malloc(sizeof(Transform));

  /* Check for failure. */
  if (newTransform == NULL)
  {
    return NULL;
  }
  // Shallow copy
  *newTransform = *other;

  return newTransform;
}

// Free the memory associated with a transform object.
// (Also, set the transform pointer to NULL.)
// Params:
//	 transform = Pointer to the transform pointer.
void TransformFree(TransformPtr * transform)
{
  if (transform != NULL)
  {
    free(*transform);
    *transform = NULL;
  }
}

// Get the transform matrix, based upon translation, rotation and scale settings.
// (HINT: If the isDirty flag is true, then recalculate the transform matrix.)
// Params:
//	 transform = Pointer to the transform object.
// Returns:
//	 If the transform pointer is valid,
//		then return a pointer to the component's matrix structure,
//		else return a NULL pointer.
Matrix2D * TransformGetMatrix(const TransformPtr transform)
{
  if (transform->isDirty)
  {
    // Formula in math: Result = Projection * translate * rotate * scale
    Matrix2D scale;
    Matrix2DScale(&scale, transform->scale.x, transform->scale.y);

    Matrix2D translate;
    Matrix2DTranslate(&translate, transform->translation.x, transform->translation.y);

    Matrix2D rotate;
    Matrix2DRotRad(&rotate, transform->rotation);

    Matrix2D result;
    Matrix2DConcat(&result, &rotate, &scale);
    
    Matrix2DConcat(&transform->matrix, &translate, &result);

    // Set dirty flag
    transform->isDirty = false;
  }

  return &transform->matrix;
}

// Get the translation of a transform component.
// Params:
//	 transform = Pointer to the transform object.
// Returns:
//	 If the transform pointer is valid,
//		then return a pointer to the component's translation structure,
//		else return a NULL pointer.
const Vector2D * TransformGetTranslation(const TransformPtr transform)
{
  if(transform != NULL)
    return &transform->translation;
  else
    return NULL;
}

// Get the rotation value of a transform component.
// Params:
//	 transform = Pointer to the transform object.
// Returns:
//	 If the transform pointer is valid,
//		then return the component's rotation value (in radians),
//		else return 0.0f.
float TransformGetRotation(const TransformPtr transform)
{
  if (transform != NULL)
    return transform->rotation;
  else
    return 0.0f;
}

// Get the scale of a transform component.
// Params:
//	 transform = Pointer to the transform object.
// Returns:
//	 If the transform pointer is valid,
//		then return a pointer to the component's scale structure,
//		else return a NULL pointer.
const Vector2D * TransformGetScale(const TransformPtr transform)
{
  if (transform != NULL)
    return &transform->scale;
  else
    return NULL;
}

// Set the translation of a transform component.
// Params:
//	 transform = Pointer to the transform component.
//	 translation = Pointer to a translation vector.
void TransformSetTranslation(TransformPtr transform, const Vector2D * translation)
{
  if (transform != NULL)
  {
    transform->translation = *translation;
    transform->isDirty = true;
  }
}

// Set the rotation of a transform component.
// Params:
//	 transform = Pointer to the transform component.
//	 rotation = The rotation value (in radians).
void TransformSetRotation(TransformPtr transform, float rotation)
{
  if (transform != NULL)
  {
    transform->rotation = rotation;
    transform->isDirty = true;
  }
}

// Set the scale of a transform component.
// Params:
//	 transform = Pointer to the transform component.
//	 translation = Pointer to a scale vector.
void TransformSetScale(TransformPtr transform, const Vector2D * scale)
{
  if (transform != NULL)
  {
    transform->scale = *scale;
    transform->isDirty = true;
  }
}

