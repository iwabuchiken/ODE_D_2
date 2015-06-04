//REF copied from: C:\ode-0.11.1\build\ODE_D_2\storage_ODE_D_2\Basic_ODE\ode.h
/*
 * myode.h
 *
 *  Created on: 2015/06/04
 *      Author: kbuchi
 */


#ifndef MYODE_H_
#define MYODE_H_

// some constants
#define DENSITY (0.5)        // density of all objects
#define GEOMSPERBODY 1       // maximum number of geometries per body
#define MAX_CONTACTS 4       // maximum number of contact points per body

typedef struct MyObject
{
    dBodyID Body;                     // the dynamics body
    dGeomID Geom[GEOMSPERBODY];       // geometries representing this body
} MyObject;




#endif /* MYODE_H_ */
