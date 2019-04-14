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
Transform::Transform(float x, float y)
{
  /* Set values */
  Vector2DSet(&this->translation, x, y);
  Vector2DSet(&this->scale, 1, 1);

  this->rotation = 0;
  this->isDirty = true;
  Matrix2DIdentity(&this->matrix);
}

// Get the transform matrix, based upon translation, rotation and scale settings.
// (HINT: If the isDirty flag is true, then recalculate the transform matrix.)
// Params:
//	 transform = Pointer to the transform object.
// Returns:
//	 If the transform pointer is valid,
//		then return a pointer to the component's matrix structure,
//		else return a NULL pointer.
const Matrix2D * Transform::getMatrix()
{
  if (this->isDirty)
  {
    // Formula in math: Result = Projection * translate * rotate * scale
    Matrix2D scale;
    Matrix2DScale(&scale, this->scale.x, this->scale.y);

    Matrix2D translate;
    Matrix2DTranslate(&translate, this->translation.x, this->translation.y);

    Matrix2D rotate;
    Matrix2DRotRad(&rotate, this->rotation);

    Matrix2D result;
    Matrix2DConcat(&result, &rotate, &scale);
    
    Matrix2DConcat(&this->matrix, &translate, &result);

    // Set dirty flag
    this->isDirty = false;
  }

  return &this->matrix;
}

// Get the translation of a transform component.
// Params:
//	 transform = Pointer to the transform object.
// Returns:
//	 If the transform pointer is valid,
//		then return a pointer to the component's translation structure,
//		else return a NULL pointer.
const Vector2D * Transform::getTranslation() const
{
	return &this->translation;
}

// Get the rotation value of a transform component.
// Params:
//	 transform = Pointer to the transform object.
// Returns:
//	 If the transform pointer is valid,
//		then return the component's rotation value (in radians),
//		else return 0.0f.
float Transform::getRotation() const
{
	return this->rotation;
}

// Get the scale of a transform component.
// Params:
//	 transform = Pointer to the transform object.
// Returns:
//	 If the transform pointer is valid,
//		then return a pointer to the component's scale structure,
//		else return a NULL pointer.
const Vector2D * Transform::getScale() const
{
    return &this->scale;
}

// Set the translation of a transform component.
// Params:
//	 transform = Pointer to the transform component.
//	 translation = Pointer to a translation vector.
void Transform::setTranslation(const Vector2D * translation)
{
    this->translation = *translation;
	this->isDirty = true;
}

// Set the rotation of a transform component.
// Params:
//	 transform = Pointer to the transform component.
//	 rotation = The rotation value (in radians).
void Transform::setRotation(float rotation)
{
	this->rotation = rotation;
	this->isDirty = true;
}

// Set the scale of a transform component.
// Params:
//	 transform = Pointer to the transform component.
//	 translation = Pointer to a scale vector.
void Transform::setScale(const Vector2D * scale)
{
	this->scale = *scale;
	this->isDirty = true;
}

