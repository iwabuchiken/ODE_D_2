/*************************************************************************
 *                                                                       *
 * Open Dynamics Engine, Copyright (C) 2001-2003 Russell L. Smith.       *
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

#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <ode/ode.h>
#include "camera.h"
#include "ode.h"
#include "console.h"
#include "matrix.h"
#include "texture.h"
#include "general.h"

using namespace std;

extern MATRIX GeomMatrix;
extern TEXTURE * texture;

// dynamics and collision objects
extern dWorldID World;             // the ode simulation world 
extern dSpaceID Space;             // collision space
extern MyObject Object;            // geom object
extern dJointGroupID contactgroup; // contact group for collisions

// this is called by dSpaceCollide when two objects in space are
// potentially colliding.
static void nearCallback(void *data, dGeomID o1, dGeomID o2)
{
    int i;

    dBodyID b1 = dGeomGetBody(o1);
    dBodyID b2 = dGeomGetBody(o2);

    dContact contact[MAX_CONTACTS];   // up to MAX_CONTACTS contacts per box-box

    for (i = 0; i < MAX_CONTACTS; i++)
    {
        contact[i].surface.mode = dContactBounce | dContactSoftCFM;
        contact[i].surface.mu = dInfinity;
        contact[i].surface.mu2 = 0;
        contact[i].surface.bounce = 0.01;
        contact[i].surface.bounce_vel = 0.1;
        contact[i].surface.soft_cfm = 0.01;
    }

    if (int numc = dCollide(o1, o2, MAX_CONTACTS, &contact[0].geom, sizeof(dContact)))
    {
        for (i = 0; i < numc; i++)
        {
            dJointID c = dJointCreateContact(World, contactgroup, contact + i);
            dJointAttach(c, b1, b2);
        }
    }
}

// Draw a geom
void DrawGeom(dGeomID g, const dReal *pos, const dReal *R, int show_aabb)
{
    if (!g)
        return;
    if (!pos)
        pos = dGeomGetPosition(g);
    if (!R)
        R = dGeomGetRotation(g);

    int type = dGeomGetClass(g);
    if (type == dBoxClass)
    {
        dReal sides[3];
        dGeomBoxGetLengths(g, sides);
        DrawBox(sides, pos, R);        // included in general.cpp
    }
}

// Simulation loop
void SimLoop()
{
    dSpaceCollide(Space, 0, &nearCallback);
    dWorldQuickStep(World, 0.05);
    dJointGroupEmpty(contactgroup);
    DrawGeom(Object.Geom[0], 0, 0, 0);
}

void InitODE()
{
    World = dWorldCreate();
    Space = dHashSpaceCreate(0);
    contactgroup = dJointGroupCreate(0);
    dCreatePlane(Space, 0, 1, 0, 0);
    dWorldSetGravity(World, 0, -1.0, 0);
    dWorldSetCFM(World, 1e-5);
    dWorldSetERP(World, 0.2);
    dWorldSetContactMaxCorrectingVel(World, 0.9);
    dWorldSetContactSurfaceLayer(World, 0);
    dWorldSetAutoDisableFlag(World, 1);

    size_t i = 0;
    dReal sides[3];
    dMass m;

    Object.Body = dBodyCreate(World);

    sides[0] = 2.0;
    sides[1] = 2.0;
    sides[2] = 2.0;

    dMatrix3 R;
    dBodySetPosition(Object.Body, 0, 10, -5);
    VECTOR tempVect(0.0, 0.0, 0.0);
    dBodySetLinearVel(Object.Body, tempVect.x, tempVect.y, tempVect.z);
    dRFromAxisAndAngle(R, dRandReal() * 2.0 - 1.0, dRandReal() * 2.0 - 1.0, dRandReal() * 2.0 - 1.0, dRandReal() * 10.0 - 5.0);
    dBodySetRotation(Object.Body, R);
    dBodySetData(Object.Body, (void*)i);
    dMassSetBox(&m, DENSITY, sides[0], sides[1], sides[2]);
    Object.Geom[0] = dCreateBox(Space, sides[0], sides[1], sides[2]);
    dGeomSetBody(Object.Geom[0], Object.Body);
    dBodySetMass(Object.Body, &m);
}

void CloseODE()
{
    dJointGroupDestroy(contactgroup);
    dSpaceDestroy(Space);
    dWorldDestroy(World);
}
