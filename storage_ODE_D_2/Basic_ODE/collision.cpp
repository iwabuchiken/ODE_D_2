// Collision detection routines and helper functions 
// (by Alan Baylis 2001, Adapted from the work of Kasper Fauerby - aka Telemachos)

#include <math.h>
#include "locmath.h"
#include "collision.h"
#include "vector.h"


void SetLength(VECTOR& v, float l)
{
 float len = sqrt(v.x*v.x + v.y*v.y + v.z*v.z);    
 v.x *= l/len;
 v.y *= l/len;
 v.z *= l/len;
} 

bool IsZeroVector(VECTOR& v)
{
  if ((v.x == 0.0f) && (v.y == 0.0f) && (v.z == 0.0f))
    return TRUE;
  return FALSE;    
}

VECTOR Wedge(VECTOR v1, VECTOR v2)
{
 VECTOR result;
 
 result.x = (v1.y * v2.z) - (v2.y * v1.z);
 result.y = (v1.z * v2.x) - (v2.z * v1.x);
 result.z = (v1.x * v2.y) - (v2.x * v1.y);     
 
 return (result);    
}

float IntersectRayPlane(VECTOR rOrigin, VECTOR rVector, VECTOR pOrigin, VECTOR pNormal) 
{
  
  float d = - (DotProduct(pNormal,pOrigin));
 
  float numer = DotProduct(pNormal,rOrigin) + d;
  float denom = DotProduct(pNormal,rVector);
  
  if (denom == 0)  // normal is orthogonal to vector, cant intersect
   return (-1.0f);
   
  return -(numer / denom);    
}

float IntersectRaySphere(VECTOR rO, VECTOR rV, VECTOR sO, float sR) 
{
   VECTOR TempVect;
   TempVect.x = sO.x - rO.x;
   TempVect.y = sO.y - rO.y;
   TempVect.z = sO.z - rO.z;
   VECTOR Q = TempVect;
   
   float c = MagnitudeVector(Q);
   float v = DotProduct(Q,rV);
   float d = sR*sR - (c*c - v*v);

   // If there was no intersection, return -1
   if (d < 0.0) return (-1.0f);

   // Return the distance to the [first] intersecting point
   return (v - sqrt(d));
}

bool CheckPointInTriangle(VECTOR point, VECTOR a, VECTOR b, VECTOR c) 
{
  
  float total_angles = 0.0f;
       
  // make the 3 vectors
  VECTOR TempVect;
  TempVect.x = point.x - a.x;
  TempVect.y = point.y - a.y;
  TempVect.z = point.z - a.z;
  VECTOR v1 = TempVect;
  TempVect.x = point.x - b.x;
  TempVect.y = point.y - b.y;
  TempVect.z = point.z - b.z;
  VECTOR v2 = TempVect;
  TempVect.x = point.x - c.x;
  TempVect.y = point.y - c.y;
  TempVect.z = point.z - c.z;
  VECTOR v3 = TempVect;
  
  v1 = GetUnitVector(v1);
  v2 = GetUnitVector(v2);
  v3 = GetUnitVector(v3);
  float Dot1 = DotProduct(v1,v2);
  if (Dot1 < -1)
    Dot1 = -1;
  if (Dot1 > 1)
    Dot1 = 1;
  total_angles += acos(Dot1);   
  float Dot2 = DotProduct(v2,v3);
  if (Dot2 < -1)
    Dot2 = -1;
  if (Dot2 > 1)
    Dot2 = 1;
  total_angles += acos(Dot2);
  float Dot3 = DotProduct(v3,v1);
  if (Dot3 < -1)
    Dot3 = -1;
  if (Dot3 > 1)
    Dot3 = 1;
  total_angles += acos(Dot3); 
     
  if (fabs(total_angles-2*pi) <= 0.005)
   return (TRUE);
     
  return(FALSE);
}

VECTOR ClosestPointOnLine(VECTOR& a, VECTOR& b, VECTOR& p) 
{
   // Determine t (the length of the vector from ‘a’ to ‘p’)
   VECTOR TempVect;
   TempVect.x = p.x - a.x;
   TempVect.y = p.y - a.y;
   TempVect.z = p.z - a.z;
   VECTOR c = TempVect;
   TempVect.x = b.x - a.x;
   TempVect.y = b.y - a.y;
   TempVect.z = b.z - a.z;
   VECTOR V = TempVect; 
      
   float d = MagnitudeVector(V);
      
   V = GetUnitVector(V);  
   double t = DotProduct(V,c);

   // Check to see if ‘t’ is beyond the extents of the line segment
   if (t < 0.0f) return (a);
   if (t > d) return (b);
 
   // Return the point between ‘a’ and ‘b’
   //set length of V to t. V is normalized so this is easy
   V.x = V.x * t;
   V.y = V.y * t;
   V.z = V.z * t;

   TempVect.x = a.x + V.x;           
   TempVect.y = a.y + V.y;           
   TempVect.z = a.z + V.z;           
   return (TempVect);    
}

VECTOR ClosestPointOnPolygon(VECTOR a, VECTOR c, VECTOR b, VECTOR p) 
{
    
  VECTOR Rab = ClosestPointOnLine(a, b, p);
  VECTOR Rbc = ClosestPointOnLine(b, c, p);
  VECTOR Rca = ClosestPointOnLine(c, a, p);
  
  VECTOR TempVect;
  TempVect.x = p.x - Rab.x;  
  TempVect.y = p.y - Rab.y;  
  TempVect.z = p.z - Rab.z;  
  float dAB = MagnitudeVector(TempVect);
  TempVect.x = p.x - Rbc.x;  
  TempVect.y = p.y - Rbc.y;  
  TempVect.z = p.z - Rbc.z;  
  float dBC = MagnitudeVector(TempVect);
  TempVect.x = p.x - Rca.x;  
  TempVect.y = p.y - Rca.y;  
  TempVect.z = p.z - Rca.z;  
  float dCA = MagnitudeVector(TempVect);
  
  float min = dAB;
  VECTOR result = Rab;
  
  if (dBC < min) 
  {
    min = dBC;
    result = Rbc;
  }
 
  if (dCA < min)
    result = Rca;
  
    return (result);    
}

bool CheckPointInSphere(VECTOR point, VECTOR sO, float sR) 
{
  VECTOR TempVect;
  TempVect.x = point.x - sO.x;
  TempVect.y = point.y - sO.y;
  TempVect.z = point.z - sO.z;
 
  float d = MagnitudeVector(TempVect);
 
  if(d <= sR) 
    return TRUE;
  return FALSE;    
}

VECTOR TangentPlaneNormalOfEllipsoid(VECTOR point, VECTOR eO, VECTOR eR)
{
 VECTOR TempVect;
 TempVect.x = point.x - eO.x;
 TempVect.y = point.y - eO.y;
 TempVect.z = point.z - eO.z;
 VECTOR p = TempVect;

 float a2 = eR.x * eR.x;
 float b2 = eR.y * eR.y;
 float c2 = eR.z * eR.z;

 
 VECTOR res;
 res.x = p.x / a2;
 res.y = p.y / b2;
 res.z = p.z / c2;

 res = GetUnitVector(res);    
 return (res);    
}

int ClassifyPoint(VECTOR point, VECTOR pO, VECTOR pN)
{
 VECTOR TempVect;
 TempVect.x = pO.x - point.x;
 TempVect.y = pO.y - point.y;
 TempVect.z = pO.z - point.z;
 VECTOR dir = TempVect;
 float d = DotProduct(dir, pN);

 if (d < -0.001f)
   return 1;
 else
   if (d > 0.001f)
     return -1;
   return 0;
}

void CheckForCollision(POLYGON* polygon, VECTOR* destination, CollisionPacket* colPackage)
{
    colPackage->foundCollision = FALSE;

      // from package
      VECTOR eRadius = colPackage->eRadius;
      VECTOR source = colPackage->sourcePoint;
      VECTOR velocity = colPackage->velocity;

      // keep a copy of this as it's needed a few times
      VECTOR normalizedVelocity = GetUnitVector(velocity);

      // intersection data
      VECTOR sIPoint;    // sphere intersection point
      VECTOR pIPoint;    // plane intersection point
      VECTOR polyIPoint; // polygon intersection point

      float distToPlaneIntersection;
      float distToSphereIntersection;

     // loop through all the polygons of the cube
     for(int i = 0; i < 12; i++)
      {

          // Get plane normal
          VECTOR pOrigin;
          pOrigin.x = polygon[i].Vertex[0].x;        //Set plane origin
          pOrigin.y = polygon[i].Vertex[0].y;
          pOrigin.z = polygon[i].Vertex[0].z;
          VECTOR pNormal = polygon[i].GetNormal();

          // Normalize plane normal
          pNormal = GetUnitVector(pNormal);

          if (DotProduct(pNormal, normalizedVelocity) <= 1.0f)
          {
              // Calculate sphere intersection point
              VECTOR eIPoint;
              eIPoint.x = source.x - pNormal.x;  //Source point + inverted plane normal
              eIPoint.y = source.y - pNormal.y;
              eIPoint.z = source.z - pNormal.z;

              // shoot ray along the velocity vector
              distToPlaneIntersection = IntersectRayPlane(eIPoint, normalizedVelocity, pOrigin, pNormal);

              // calculate plane intersection point
              pIPoint.x = eIPoint.x + distToPlaneIntersection * normalizedVelocity.x;
              pIPoint.y = eIPoint.y + distToPlaneIntersection * normalizedVelocity.y;
              pIPoint.z = eIPoint.z + distToPlaneIntersection * normalizedVelocity.z;

              int pClass = ClassifyPoint(eIPoint, pOrigin, pNormal);

              // find the plane intersection point
              if (pClass == -1)
              {               // plane spans sphere

                // find plane intersection point by shooting a ray from the
                // sphere intersection point along the planes normal.
                distToPlaneIntersection = IntersectRayPlane(eIPoint, pNormal, pOrigin, pNormal);

                // calculate plane intersection point
                pIPoint.x = eIPoint.x + distToPlaneIntersection * pNormal.x;
                pIPoint.y = eIPoint.y + distToPlaneIntersection * pNormal.y;
                pIPoint.z = eIPoint.z + distToPlaneIntersection * pNormal.z;
              }

              // find polygon intersection point. By default we assume its equal to the
              // plane intersection point. In that case we already know the distance to it.
              polyIPoint = pIPoint;
              distToSphereIntersection = distToPlaneIntersection;

              VECTOR a, b, c;
              a.x = polygon[i].Vertex[0].x;
              a.y = polygon[i].Vertex[0].y;
              a.z = polygon[i].Vertex[0].z;
              b.x = polygon[i].Vertex[1].x;
              b.y = polygon[i].Vertex[1].y;
              b.z = polygon[i].Vertex[1].z;
              c.x = polygon[i].Vertex[2].x;
              c.y = polygon[i].Vertex[2].y;
              c.z = polygon[i].Vertex[2].z;

              if(!CheckPointInTriangle(pIPoint, a, b, c))
              {
                // if not in triangle
                    polyIPoint = ClosestPointOnPolygon(a, b, c, pIPoint);

                    VECTOR invertednormalizedVelocity;
                    invertednormalizedVelocity.x = -normalizedVelocity.x;
                    invertednormalizedVelocity.y = -normalizedVelocity.y;
                    invertednormalizedVelocity.z = -normalizedVelocity.z;
                    distToSphereIntersection = IntersectRaySphere(polyIPoint, invertednormalizedVelocity, source, 1.0f);

                    // we cannot know if the ray will actually hit the sphere so we need this check
                    if (distToSphereIntersection > 0)
                    {
                          // calculate true ellipsoid intersection point
                          eIPoint.x = polyIPoint.x + distToSphereIntersection * invertednormalizedVelocity.x;
                          eIPoint.y = polyIPoint.y + distToSphereIntersection * invertednormalizedVelocity.y;
                          eIPoint.z = polyIPoint.z + distToSphereIntersection * invertednormalizedVelocity.z;
                    }
              }

              // any chance of hit ?
              if ((distToSphereIntersection > 0) && (distToSphereIntersection <= MagnitudeVector(velocity)))
              {
                      // if first hit, or closest hit so far
                     if ((colPackage->foundCollision == FALSE) || (distToSphereIntersection < colPackage->nearestDistance))
                     {
                           colPackage->nearestDistance = distToSphereIntersection;
                           colPackage->nearestSphereIntersectionPoint = eIPoint;
                           colPackage->nearestPolygonIntersectionPoint = polyIPoint;
                           colPackage->foundCollision = TRUE;
                     }
              }
        }
    }

    if (!colPackage->foundCollision)
        return;

    //Collision response

    // OK, first task is to move close to where we hit something :
      VECTOR newSourcePoint;

      // only update if we are not already very close
      if (colPackage->nearestDistance >= epsilon)
      {

            VECTOR V = velocity;
              SetLength(V, colPackage->nearestDistance-epsilon);
            newSourcePoint.x = source.x + V.x;
            newSourcePoint.y = source.y + V.y;
            newSourcePoint.z = source.z + V.z;
      }
    else
            newSourcePoint = source;

    // Determine the sliding plane (we do this now, because we're about to
      // change sourcePoint)
      VECTOR slidePlaneOrigin = colPackage->nearestPolygonIntersectionPoint;
      VECTOR slidePlaneNormal;
      slidePlaneNormal.x = newSourcePoint.x - colPackage->nearestPolygonIntersectionPoint.x;
      slidePlaneNormal.y = newSourcePoint.y - colPackage->nearestPolygonIntersectionPoint.y;
      slidePlaneNormal.z = newSourcePoint.z - colPackage->nearestPolygonIntersectionPoint.z;
        // We now project the destination point onto the sliding plane
      float l = IntersectRayPlane(*destination, slidePlaneNormal, slidePlaneOrigin, slidePlaneNormal);
     VECTOR newDestinationPoint;
      newDestinationPoint.x = destination->x + l * slidePlaneNormal.x;
      newDestinationPoint.y = destination->y + l * slidePlaneNormal.y;
      newDestinationPoint.z = destination->z + l * slidePlaneNormal.z;

    VECTOR newVelocityVector;
      newVelocityVector.x = newDestinationPoint.x - colPackage->nearestPolygonIntersectionPoint.x;
      newVelocityVector.y = newDestinationPoint.y - colPackage->nearestPolygonIntersectionPoint.y;
     newVelocityVector.z = newDestinationPoint.z - colPackage->nearestPolygonIntersectionPoint.z;

    destination->x = newSourcePoint.x + newVelocityVector.x;
      destination->y = newSourcePoint.y + newVelocityVector.y;
      destination->z = newSourcePoint.z + newVelocityVector.z;
    colPackage->sourcePoint = newSourcePoint;
    colPackage->velocity = newVelocityVector;
    colPackage->foundCollision = 0;
        CheckForCollision(polygon, destination, colPackage);
}
