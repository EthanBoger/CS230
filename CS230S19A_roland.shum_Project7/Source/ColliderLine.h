//------------------------------------------------------------------------------
//
// File Name:	ColliderLine.h
// Author(s):	Doug Schilling (dschilling)
// Project:		Project 6
// Course:		CS230S19
//
// Copyright © 2019 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once
#include "stdafx.h"
#include "Collider.h"
//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

typedef struct Vector2D	Vector2D;

//------------------------------------------------------------------------------
// Public Consts:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

// An example of the structures to be defined in ColliderLine.c.
typedef struct ColliderLineSegment
{
	// A single line segment (P0 and P1).
	Vector2D	point[2];
} ColliderLineSegment;

typedef struct ColliderLine : public Collider
{
private:
	static const int cLineSegmentMax = 100;

	typedef struct CollisionData
	{
		Vector2D collisionPoint;
		Vector2D reflectionVector;
		Vector2D newEndPoint;
	}CollisionData, *CollisionDataPtr;

private:
	// The number of line segments in the list.
	unsigned int		lineCount;

	// The individual line segments.
	// (NOTE: Make sure to allocate enough memory for all line segments!)
	ColliderLineSegment	lineSegments[cLineSegmentMax];



public:
	ColliderLine();

	void AddLineSegment(const Vector2D * p0, const Vector2D * p1);

	static bool PointLineIntersection(Vector2D const *circlePosition, Vector2D const *velocity,
		ColliderLineSegment const *segment, float radius, Vector2D *point);

	virtual ColliderPtr Clone(GameObjectPtr parent);

	static bool LineIsCollidingWithCircle(ColliderLinePtr collider, ColliderCirclePtr other);

private:
	static bool StaticPointInStaticCircle(Vector2D const *circlePosition, float radius, Vector2D const * point);
	static CollisionData CaclCollisionResponse(Vector2D const *circlePosition, Vector2D const *velocity,
		ColliderLineSegment const *segment, Vector2D const *collisionPoint);

} ColliderLine, *ColliderLinePtr;


//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
