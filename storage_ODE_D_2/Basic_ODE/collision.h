// Collision detection routines and helper functions 
// (by Alan Baylis 2001, Adapted from the work of Kasper Fauerby - aka Telemachos)

#ifndef COLLISION_H
#define COLLISION_H

#include "vector.h"
#include "polygon.h"
#include "shared.h"
#include "locmath.h"

extern float radian;
extern float pi;
extern float epsilon;

struct CollisionPacket
{
  // data about player movement
 VECTOR velocity;
 VECTOR sourcePoint;
  
 // radius of ellipsoid.  
 VECTOR eRadius; 
  
 // for error handling  
 VECTOR lastSafePosition;
 bool stuck; 
    
 // data for collision response 
 bool foundCollision;
 float    nearestDistance; // nearest distance to hit
 VECTOR nearestSphereIntersectionPoint; // on sphere
 VECTOR nearestPolygonIntersectionPoint; // on polygon
};

void SetLength(VECTOR& v, float l);
bool IsZeroVector(VECTOR& v);
VECTOR Wedge(VECTOR v1, VECTOR v2);
float IntersectRayPlane(VECTOR rOrigin, VECTOR rVector, VECTOR pOrigin, VECTOR pNormal);
float IntersectRaySphere(VECTOR rO, VECTOR rV, VECTOR sO, float sR);
bool CheckPointInTriangle(VECTOR point, VECTOR a, VECTOR b, VECTOR c);
VECTOR ClosestPointOnLine(VECTOR& a, VECTOR& b, VECTOR& p);
VECTOR ClosestPointOnPolygon(VECTOR a, VECTOR c, VECTOR b, VECTOR p);
bool CheckPointInSphere(VECTOR point, VECTOR sO, float sR);
VECTOR TangentPlaneNormalOfEllipsoid(VECTOR point, VECTOR eO, VECTOR eR);
int ClassifyPoint(VECTOR point, VECTOR pO, VECTOR pN);
void CheckForCollision(POLYGON* polygon, VECTOR* destination, CollisionPacket* colPackage);

#endif
