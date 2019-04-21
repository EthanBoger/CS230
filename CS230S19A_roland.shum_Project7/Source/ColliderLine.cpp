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
#include "Stub.h"

#include <AEEngine.h>
#include "ColliderLine.h"
#include "Vector2D.h"
#include "Transform.h"
#include "GameObject.h"
#include "Matrix2D.h"
#include "Trace.h"
#include "ColliderCircle.h"
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

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Dynamically allocate a new (line) collider component.
// (Hint: Use calloc() to ensure that all member variables are initialized to 0.)
// (Hint: Make sure to initialize the ColliderType and memorySize correctly.)
ColliderLine::ColliderLine() : Collider(ColliderTypeLine)
{

}

ColliderLinePtr ColliderLine::Clone(void) const
{
	ColliderLinePtr newLine = new ColliderLine(*this); 
	return newLine;
}

// Add a line segment to the line collider's line segment list.
// Params:
//	 collider = Pointer to the line collider component.
//	 p0 = The line segment's starting position.
//	 p1 = The line segment's ending position.
void ColliderLine::AddLineSegment(const Vector2D * p0, const Vector2D * p1)
{
	this->lineSegments[this->lineCount].point[0] = *p0;
	this->lineSegments[this->lineCount].point[1] = *p1;
	this->lineCount++;
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

bool ColliderLine::PointLineIntersection(Vector2D const *circlePosition, Vector2D const *velocity, ColliderLineSegment const *segment,
	float radius, Vector2D *point)
{
	// Get the vector AB
	Vector2D AB;
	Vector2DSub(&AB, &segment->point[0], &segment->point[1]);

	// Get the point if there is no collision
	Vector2D Be;
	Vector2DAdd(&Be, circlePosition, velocity);

	// Get the normal vector of the line segment.
	Vector2D normalVector;
	normalVector.x = AB.y;
	normalVector.y = -AB.x;

	// Normalize it
	Vector2DNormalize(&normalVector, &normalVector);

	// ti = (n * P0 - n * Bs) + D
	//		-----------------
	//			n * v
	
	float numerator = Vector2DDotProduct(&normalVector, &segment->point[0]) - Vector2DDotProduct(&normalVector, circlePosition);
	float denominator = Vector2DDotProduct(&normalVector, velocity);

	UNREFERENCED_PARAMETER(radius);

	float time = (numerator )/ denominator;

	// Check if t is between 0 and 1
	if (0 <= time && time <= 1)
	{
		// We might have an intersection. Check for collision point.
		Vector2D movementWithTime;
		Vector2DScale(&movementWithTime, velocity, time);
		Vector2DAdd(point, &movementWithTime, circlePosition);

		// Check for if its an infinite wall collision that is happening
		Vector2D left, BA;
		Vector2DNeg(&BA, &AB);

		// (Bi - P0) * (P1 - P0) < 0
		Vector2DSub(&left, point, &segment->point[0]);
		if (Vector2DDotProduct(&left, &BA) < 0)
		{
			return false;
		}
		
		// Reuse the left varaible.
		// (Bi - P1) * ( P0 - P1 ) < 0
		Vector2DSub(&left, point, &segment->point[1]);
		Vector2DNeg(&BA, &AB);
		if (Vector2DDotProduct(&left, &AB) < 0)
		{
			return false;
		}
		return true;
	}
	else
	{
		// no intersection
		return false;
	}
}

bool ColliderLine::StaticPointInStaticCircle(Vector2D const *circlePosition, float radius, Vector2D const * point)
{
	float disSquared = Vector2DSquareDistance(circlePosition, point);
	if (disSquared < radius * radius)
		return true;
	return false;
}

ColliderLine::CollisionData ColliderLine::CaclCollisionResponse(Vector2D const *circlePosition, Vector2D const *velocity, ColliderLineSegment const *segment,
	Vector2D const *collisionPoint)
{
	// Get the vector AB
	Vector2D AB;
	Vector2DSub(&AB, &segment->point[0], &segment->point[1]);

	// Get the normal vector of the line segment.
	Vector2D normalVector;
	normalVector.x = AB.y;
	normalVector.y = -AB.x;
	Vector2DNormalize(&normalVector, &normalVector);

	// This is the point if collision did not happen
	Vector2D Be;
	Vector2DAdd(&Be, velocity, circlePosition);

	// i = Be - Bi, Bi is point of collision, Be is depth penetration, 
	Vector2D i;
	Vector2DSub(&i, &Be, collisionPoint);

	// r = i - 2(i * n) n, r is reflection vector
	Vector2D r;
	float iDOTn = Vector2DDotProduct(&i, &normalVector) * 2;
	Vector2D reflectNormal;
	Vector2DScale(&reflectNormal, &normalVector, iDOTn);
	Vector2DSub(&r, &i, &reflectNormal);

	// Find the new point after reflection.
	Vector2D resultPoint;
	Vector2DAdd(&resultPoint, collisionPoint, &r);

	// Find the new velocity, r normalized.
	Vector2DNormalize(&r, &r);

	// Prepare the struct to send out.
	CollisionData data;
	data.collisionPoint = *collisionPoint;
	data.reflectionVector = r;
	data.newEndPoint = resultPoint;

	return data;
}

bool ColliderLine::LineIsCollidingWithCircle(ColliderLinePtr collider, ColliderCirclePtr other)
{
	if (collider == NULL || other == NULL)
		return false;
	if (collider->type != ColliderTypeLine || other->type != ColliderTypeCircle)
		return false;

	bool returnValue = false;

	// Get the translations of the colliders.
	TransformPtr thisTransform, otherTransform;
	thisTransform = collider->parent->getTransform();
	otherTransform = other->parent->getTransform();

	// We need to access each line segment.
	ColliderLinePtr lineCollider = (ColliderLinePtr)collider;

	// Get the radius of the circle collider.
	float radius = other->GetRadius();

	Vector2D collisionPoint;
	Vector2D otherTranslation = *otherTransform->getTranslation();

	// Get the velocity of my circle object.
	Vector2D velocity = *other->parent->getPhysics()->getVelocity();

	// Scale the velocity by DT
	Vector2DScale(&velocity, &velocity, (float)AEFrameRateControllerGetFrameTime());

	// For each line segment, check if it collides.
	for (unsigned i = 0; i < lineCollider->lineCount; i++)
	{
		// Check for collision
		if (PointLineIntersection(&otherTranslation, &velocity, &lineCollider->lineSegments[i],
			radius, &collisionPoint))
		{
			// Collision response
			CollisionData data = CaclCollisionResponse(&otherTranslation, &velocity, &lineCollider->lineSegments[i], &collisionPoint);

			// Set the circle to the new end point, and give it a new velocity.
			otherTransform->setTranslation(&data.newEndPoint);

			PhysicsPtr physics = other->parent->getPhysics();
			// Get my previous speed.
			float speed = Vector2DLength(physics->getVelocity());

			// Scale my unit vector of reflection to speed.
			Vector2D newVelocity;
			Vector2DScale(&newVelocity, &data.reflectionVector, speed);

			// Turn the object so that its facing its new reflected vector
			float rotationVector = Vector2DToAngleRad(&data.reflectionVector);
			otherTransform->setRotation(rotationVector);
			// Set the new velocity.
			physics->setVelocity(&newVelocity);

			returnValue = true;
		}
	}
	return returnValue;
}