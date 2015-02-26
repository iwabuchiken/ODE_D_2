/*************************************************************************
 *                                                                       *
 * Open Dynamics Engine, Copyright (C) 2001,2002 Russell L. Smith.       *
 * All rights reserved.  Email: russ@q12.org   Web: www.q12.org          *
 *                                                                       *
 * This library is free software; you can redistribute it and/or         *
 * modify it under the terms of EITHER:                                  *
 *   (1) The GNU Lesser General Public License as published by the Free  *
 *       Software Foundation; either version 2.1 of the License, or (at  *
 *       your option) any later version. The text of the GNU Lesser      *
 *       General Public License is included with this library in the     *
 *       file LICENSE.TXT.                                               *
 *   (2) The BSD-style license that is included with this library in     *
 *       the file LICENSE-BSD.TXT.                                       *
 *                                                                       *
 * This library is distributed in the hope that it will be useful,       *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the files    *
 * LICENSE.TXT and LICENSE-BSD.TXT for more details.                     *
 *                                                                       *
 *************************************************************************/

/*

buggy with suspension.
this also shows you how to use geom groups.

*/


#include <ode/ode.h>
#include <drawstuff/drawstuff.h>
#include "texturepath.h"

#include <math.h>

#ifdef _MSC_VER
#pragma warning(disable:4244 4305)  // for VC++, no precision loss complaints
#endif

// select correct drawing functions

#ifdef dDOUBLE
#define dsDrawBox dsDrawBoxD
#define dsDrawSphere dsDrawSphereD
#define dsDrawCylinder dsDrawCylinderD
#define dsDrawCapsule dsDrawCapsuleD
#endif


static dWorldID world; // a physics world
dBodyID ball;      // an ball
//const dReal radius = 3, mass = 1.0;// radius(m)、weight(kg) of an ball
dReal radius = 0.2;// radius(m)、weight(kg) of an ball
const dReal mass = 1.0;// radius(m)、weight(kg) of an ball
//const dReal mass = 3.0;// radius(m)、weight(kg) of an ball

///////////////////////////////
//
// vars
//
 ///////////////////////////////
float color[3] = {1.0, 0, 1.0};
//float color[3] = {1.0, 1.0, 0};

float count = 0;

float head = 90.0;
float pitch = -15.0;
float roll = 0.0;

float pos_X = 0.0;
float pos_Y = -7.0;
//float pos_Y = -3.0;
float pos_Z = 3.0;

float grav_Z = -0.001;

float step = 0.05;

dReal pos0_Z = 4.0;


// Simulation loop
static void simLoop (int pause)
{
	const dReal *pos,*R;//position, rotation matrix
	dWorldStep(world, step);// Step a simulation world, time step is 0.05 [s]
//	dWorldStep(world,0.01);// Step a simulation world, time step is 0.05 [s]
//	dWorldStep(world,0.05);// Step a simulation world, time step is 0.05 [s]

//	dsSetColor(1.0,0.0,0.0);  // Set color  (red, green, blue) value is from 0 to 1.0
//	dsSetColor(color[0], color[1], color[2]);  // Set color  (red, green, blue) value is from 0 to 1.0
	dsSetColor((float)sin(count), color[1], color[2]);  // Set color  (red, green, blue) value is from 0 to 1.0

	count += M_PI / 1080;	// 360 * 3

	pos = dBodyGetPosition(ball);// Get a position
	R = dBodyGetRotation(ball);// Get a rotation

	dsDrawSphere(pos,R,radius);// Draw a sphere

	radius += 0.0001;

}

// Start function
void start()
{
	// Set a camera
	static float xyz[3] = {pos_X, pos_Y, pos_Z};   // View position (x, y, z [m])
//	static float xyz[3] = {0.0,-3.0, pos_Z};   // View position (x, y, z [m])

//	static float xyz[3] = {0.0,-3.0,1.0};   // View position (x, y, z [m])
	static float hpr[3] = {head, pitch, roll};  // View direction （head, pitch, roll[°]）
//	static float hpr[3] = {head,0.0,0.0};  // View direction （head, pitch, roll[°]）
//	static float hpr[3] = {90.0,0.0,0.0};  // View direction （head, pitch, roll[°]）
	dsSetViewpoint (xyz,hpr);// Set a view point
}

// main function
int main (int argc, char **argv)
{
	dReal x0 = 0.0, y0 = 0.0, z0 = pos0_Z; // initial position of an ball[m]
//	dReal x0 = 0.0, y0 = 0.0, z0 = 3.0; // initial position of an ball[m]
//	dReal x0 = 0.0, y0 = 0.0, z0 = 1.0; // initial position of an ball[m]
//  dReal x0 = 0.0, y0 = 0.0, dReal z0 = 1.0; // initial position of an ball[m]
	dMass m1; // mass parameter

	// for drawstuff
	dsFunctions fn;//drawstuff structure
	fn.version = DS_VERSION;  // the version of the drawstuff
	fn.start = &start;      // start function
	fn.step = &simLoop;        // step function
	fn.command = NULL;   // no command function for keyboard
	fn.stop  = NULL;     // no stop function
	fn.path_to_textures = "../../drawstuff/textures";//path to the texture

	dInitODE();      // Initialize ODE
	world = dWorldCreate();  // Create a dynamic world
//	dWorldSetGravity(world,0.0001,0.0001,-0.0001);// Set gravity（x, y, z)
//	dWorldSetGravity(world,0,0,0.0001);// Set gravity（x, y, z)
//	dWorldSetGravity(world,0,0,-0.0001);// Set gravity（x, y, z)
//	dWorldSetGravity(world,0,0,-0.003);// Set gravity（x, y, z)
//	dWorldSetGravity(world,0,0,-0.001);// Set gravity（x, y, z)
	dWorldSetGravity(world,0,0, grav_Z);// Set gravity（x, y, z)

	//Create a ball
	ball = dBodyCreate(world); //  Crate a rigid body
	dMassSetZero(&m1); // Initialize mass parameters
	dMassSetSphereTotal(&m1,mass,radius);// Calculate a mass parameter
	dBodySetMass(ball,&m1); // Set a mass parameter
	dBodySetPosition(ball, x0, y0, z0);//Set a position(x, y, z)

	// Simulation loop
	// argc, argv are argument of main function. Window size is  352 x 288 [pixel]
	// fn is a structure of drawstuff
	dsSimulationLoop (argc,argv,352,288,&fn);

	dWorldDestroy(world);// Destroy the world
	dCloseODE();                 // Close ODE

	printf("done\n");


	return 0;
}
