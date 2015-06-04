#ifndef _ODE_H
#define _ODE_H

#include <ode/ode.h>    // ode library header

// some constants
#define DENSITY (0.5)        // density of all objects
#define GEOMSPERBODY 1       // maximum number of geometries per body
#define MAX_CONTACTS 4       // maximum number of contact points per body

typedef struct MyObject
{
    dBodyID Body;                     // the dynamics body
    dGeomID Geom[GEOMSPERBODY];       // geometries representing this body
} MyObject;

void DrawGeom (dGeomID, const dReal*, const dReal*, int);
void SimLoop();
void InitODE();
void CloseODE();

#endif

