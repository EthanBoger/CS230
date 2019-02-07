//------------------------------------------------------------------------------
//
// File Name:	Stub.c
// Author(s):	Roland Shum
// Project:		MyGame
// Course:		CS230S19
//
// Copyright © 2019 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Vector2D.h"
#include <assert.h>
#include <AEVec2.h>
#include <math.h>
#include <float.h>
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
int float_isequal(float one, float two);
//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Initialize the ...
void TestVector2D(void)
{
  Vector2D one, two, three;

  /* Test vector set. */
  Vector2DSet(&one, 5, 5);
  assert(one.x == 5 && one.y == 5);

  /* Test vector zero. */
  Vector2DZero(&two);
  assert(two.x == 0 && two.y == 0);

  /* Test Neg */
  Vector2DNeg(&one, &one);
  assert(one.x == -5 && one.y == -5);

  /* Test Add. */
  Vector2DSet(&three, 10, 10);
  Vector2DAdd(&one, &one, &two);
  assert(one.x == -5 && one.y == -5);
  Vector2DAdd(&two, &three, &one);
  assert(two.x == 5 && two.y == 5);

  /* Test Sub.*/
  Vector2DSub(&three, &three, &two);
  assert(three.x == 5 && three.y == 5);

  /* Test Normalize. */
  Vector2DSet(&one, 0, 5);
  Vector2DNormalize(&one, &one);
  assert(one.x == 0 && one.y == 1);

  /* Test Scale */
  Vector2DScale(&one, &one, 5);
  assert(one.x == 0 && one.y == 5);

  /* Test Scale Add */
  Vector2DSet(&two, 2, 2);
  Vector2DScaleAdd(&three, &one, &two, 2);
  assert(three.x == 2 && three.y == 12);

  /* Test Scale Sub */
  Vector2DSet(&two, 2, 2);
  Vector2DScaleSub(&three, &one, &two, 2);
  assert(three.x == -2 && three.y == 8);

  /* Test Length */
  Vector2DSet(&one, 0, 1);
  float length = Vector2DLength(&one);
  assert(length == 1);

  /* Test distance */
  Vector2DSet(&one, 0, 5);
  Vector2DSet(&two, 0, 10);
  float distance = Vector2DDistance(&one, &two);
  assert(distance == 5);

  /* Test distance square */
  Vector2DSet(&one, 0, 5);
  Vector2DSet(&two, 0, 10);
  distance = Vector2DSquareDistance(&one, &two);
  assert(distance == 25);

  /* Test dot product. */
  float dot = Vector2DDotProduct(&one, &two);
  assert(dot == 50);

  /* Test Vector to angle. */
  Vector2DSet(&one, 1, 5);
  float angle = Vector2DToAngleRad(&one);
  assert(float_isequal(angle, 1.3734f));

  /* Test vector from angle. */
  AEVec2 AEvecAngle, angle2;
  Vector2DFromAngleDeg(&two, 78.69007f);
  AEVec2FromAngle(&AEvecAngle, (78.69007f * PI) / 180.f);
  assert(float_isequal(AEvecAngle.x, two.x) && float_isequal(AEvecAngle.y, two.y));

  /* Test vector from angle rad. */
  AEVec2FromAngle(&angle2, 1.3734f);
  Vector2DFromAngleRad(&three, 1.3734f);
  assert(float_isequal(three.x, angle2.x) && float_isequal(three.y, angle2.y));


}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

int float_isequal(float one, float two)
{
  return (fabs(one - two) <= 0.00001);
}