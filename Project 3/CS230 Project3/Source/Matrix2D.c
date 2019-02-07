//------------------------------------------------------------------------------
//
// File Name:	Matrix2D.c
// Author(s):	Roland Shum
// Project:		MyGame
// Course:		CS230S19
//
// Copyright © 2019 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Matrix2D.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <assert.h>

//------------------------------------------------------------------------------
// Private Consts:
//------------------------------------------------------------------------------
const int matrix_size = 3;
//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------
static Matrix2D temp;
//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------
static void Swap(float *one, float *two);
//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------
// This function sets the matrix equal to the identity matrix.
void Matrix2DIdentity(Matrix2D *pResult)
{
  // Crash if result is null.
  assert(pResult != NULL);

  // If not NULL
  if (pResult != NULL)
  {
    // Set everything to 0.
    memset(pResult, 0, sizeof(Matrix2D));

    // Set the specific parts to 1.
    pResult->m[0][0] = 1;
    pResult->m[1][1] = 1;
    pResult->m[2][2] = 1;
  }
}

// This function calculates the transpose matrix of Mtx and saves it in the result matrix.
// (NOTE: Care must be taken when pResult = pMtx.)
void Matrix2DTranspose(Matrix2D *pResult, Matrix2D *pMtx)
{
  // Initialize the temp.
  Matrix2DIdentity(&temp);

  // Copy the matrix to transpose to temp.
  temp = *pMtx;

  // Now we operate on the temp.
  for (int i = 0; i < matrix_size; i++)
    for (int j = i + 1; j < matrix_size; j++)
      Swap(&temp.m[i][j], &temp.m[j][i]);

  // Copy back to result.
  *pResult = temp;
}

// This function multiplies Mtx0 with Mtx1 and saves the result in Result.
// Result = Mtx0*Mtx1
// (NOTE: Care must be taken when pResult = either pMtx0 or pMtx1.)
void Matrix2DConcat(Matrix2D *pResult, Matrix2D *pMtx0, Matrix2D *pMtx1)
{
  // Initialize the temp.
  Matrix2DIdentity(&temp);

  int i, j, k;
  for (i = 0; i < matrix_size; i++)
  {
    for (j = 0; j < matrix_size; j++)
    {
      temp.m[i][j] = 0;
      for (k = 0; k < matrix_size; k++)
        temp.m[i][j] += pMtx0->m[i][k] * pMtx1->m[k][j];
    }
  }
  // Copy the result over to temp.
  *pResult = temp;
}

// This function creates a translation matrix from x & y and saves it in Result.
// (Hint: This function must initialize all matrix values.)
void Matrix2DTranslate(Matrix2D *pResult, float x, float y)
{
  //[1][0][x]
  //[0][1][y]
  //[0][0][1]
  // Initialize pResult
  Matrix2DIdentity(pResult);

  pResult->m[0][2] = x;
  pResult->m[1][2] = y;
}

// This function creates a scaling matrix from x & y and saves it in Result.
// (Hint: This function must initialize all matrix values.)
void Matrix2DScale(Matrix2D *pResult, float x, float y)
{
  //[x][0][0]
  //[0][y][0]
  //[0][0][1]
  // Initialize pResult
  Matrix2DIdentity(pResult);

  pResult->m[0][0] = x;
  pResult->m[1][1] = y;
}

// This matrix creates a rotation matrix from "Angle" whose value is in degrees.
// (Hint: This function must initialize all matrix values.)
// Save the resultant matrix in Result.
// Converting from degrees to radians can be performed as follows:
//	 radians = (angle * M_PI) / 180.0f
// M_PI is defined in "math.h", which may be included as follows:
//   #define _USE_MATH_DEFINES
//   #include <math.h>
void Matrix2DRotDeg(Matrix2D *pResult, float angle)
{
  //[cos a][-sin a][0]
  //[sin a][cos a ][0]
  //[0    ][0     ][1]
  // Initialize pResult
  Matrix2DIdentity(pResult);

  // Change to radians.
  angle = (angle * (float)M_PI) / 180.f;

  Matrix2DRotRad(pResult, angle);
}

// This matrix creates a rotation matrix from "Angle" whose value is in radians.
// (Hint: This function must initialize all matrix values.)
// Save the resultant matrix in Result.
void Matrix2DRotRad(Matrix2D *pResult, float angle)
{
  //[cos a][-sin a][0]
  //[sin a][cos a ][0]
  //[0    ][0     ][1]
  pResult->m[0][0] = (float)cos(angle);
  pResult->m[0][1] = -(float)sin(angle);

  pResult->m[1][0] = (float)sin(angle);
  pResult->m[1][1] = (float)cos(angle);
}

// This function multiplies the matrix Mtx with the vector Vec and saves the result in Result.
// Result = Mtx * Vec.
// (NOTE: Care must be taken when pResult = pVec.)
void Matrix2DMultVec(Vector2D *pResult, Matrix2D *pMtx, Vector2D *pVec)
{
  /* Initialize the vector.*/
  static Vector2D resultVector;

  // Set the x.
  resultVector.x = pVec->x * pMtx->m[0][0] + pVec->y * pMtx->m[0][1];

  // Set the y.
  resultVector.y = pVec->x * pMtx->m[1][0] + pVec->y * pMtx->m[1][1];

  // Copy the temp to pResult.
  *pResult = resultVector;
}


//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------
static void Swap(float *one, float *two)
{
  float temp_f = *one;
  *one = *two;
  *two = temp_f;
}
