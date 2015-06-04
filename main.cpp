//REF http://www.alsprogrammingresource.com/basic_ode.html

#include <ode/ode.h>
#include <drawstuff/drawstuff.h>

#include "myode.h"

MATRIX GeomMatrix;

MyObject Object;

dWorldID World;

dSpaceID Space;

dJointGroupID contactgroup;

#define GEOMSPERBODY 1  // maximum number of geometries per body



struct MyObject

{

    dBodyID Body;  // the dynamics body

    dGeomID Geom[GEOMSPERBODY];  // geometries representing this body

};

void InitODE()

{

    // Create a new, empty world and assign its ID number to World. Most applications will only need one world.

    World = dWorldCreate();



    // Create a new collision space and assign its ID number to Space, passing 0 instead of an existing dSpaceID.

       // There are three different types of collision spaces we could create here depending on the number of objects

       // in the world but dSimpleSpaceCreate is fine for a small number of objects. If there were more objects we

       // would be using dHashSpaceCreate or dQuadTreeSpaceCreate (look these up in the ODE docs)

    Space = dSimpleSpaceCreate(0);



    // Create a joint group object and assign its ID number to contactgroup. dJointGroupCreate used to have a

       // max_size parameter but it is no longer used so we just pass 0 as its argument.

    contactgroup = dJointGroupCreate(0);



    // Create a ground plane in our collision space by passing Space as the first argument to dCreatePlane.

       // The next four parameters are the planes normal (a, b, c) and distance (d) according to the plane

       // equation a*x+b*y+c*z=d and must have length 1

    dCreatePlane(Space, 0, 1, 0, 0);



    // Now we set the gravity vector for our world by passing World as the first argument to dWorldSetGravity.

       // Earth's gravity vector would be (0, -9.81, 0) assuming that +Y is up. I found that a lighter gravity looked

       // more realistic in this case.

    dWorldSetGravity(World, 0, -1.0, 0);



    // These next two functions control how much error correcting and constraint force mixing occurs in the world.

       // Don't worry about these for now as they are set to the default values and we could happily delete them from

       // this example. Different values, however, can drastically change the behaviour of the objects colliding, so

       // I suggest you look up the full info on them in the ODE docs.

    dWorldSetERP(World, 0.2);

    dWorldSetCFM(World, 1e-5);



    // This function sets the velocity that interpenetrating objects will separate at. The default value is infinity.

    dWorldSetContactMaxCorrectingVel(World, 0.9);

    // This function sets the depth of the surface layer around the world objects. Contacts are allowed to sink into

       // each other up to this depth. Setting it to a small value reduces the amount of jittering between contacting

       // objects, the default value is 0.

    dWorldSetContactSurfaceLayer(World, 0.001);



    // To save some CPU time we set the auto disable flag to 1. This means that objects that have come to rest (based

       // on their current linear and angular velocity) will no longer participate in the simulation, unless acted upon

       // by a moving object. If you do not want to use this feature then set the flag to 0. You can also manually enable

       // or disable objects using dBodyEnable and dBodyDisable, see the docs for more info on this.

    dWorldSetAutoDisableFlag(World, 1);



    // This brings us to the end of the world settings, now we have to initialize the objects themselves.



    // Create a new body for our object in the world and get its ID.

    Object.Body = dBodyCreate(World);



    // Next we set the position of the new body

    dBodySetPosition(Object.Body, 0, 10, -5);



    // Here I have set the initial linear velocity to stationary and let gravity do the work, but you can experiment

       // with the velocity vector to change the starting behaviour. You can also set the rotational velocity for the new

       // body using dBodySetAngularVel which takes the same parameters.

    VECTOR tempVect(0.0, 0.0, 0.0);

    dBodySetLinearVel(Object.Body, tempVect.x, tempVect.y, tempVect.z);



    // To start the object with a different rotation each time the program runs we create a new matrix called R and use

       // the function dRFromAxisAndAngle to create a random initial rotation before passing this matrix to dBodySetRotation.

    dMatrix3 R;

    dRFromAxisAndAngle(R, dRandReal() * 2.0 - 1.0,

                          dRandReal() * 2.0 - 1.0,

                          dRandReal() * 2.0 - 1.0,

                          dRandReal() * 10.0 - 5.0);

    dBodySetRotation(Object.Body, R);



    // At this point we could add our own user data using dBodySetData but in this example it isn't used.

    size_t i = 0;

    dBodySetData(Object.Body, (void*)i);



    // Now we need to create a box mass to go with our geom. First we create a new dMass structure (the internals

       // of which aren't important at the moment) then create an array of 3 float (dReal) values and set them

       // to the side lengths of our box along the x, y and z axes. We then pass the both of these to dMassSetBox with a

       // pre-defined DENSITY value of 0.5 in this case.

    dMass m;

    dReal sides[3];

    sides[0] = 2.0;

    sides[1] = 2.0;

    sides[2] = 2.0;

    dMassSetBox(&m, DENSITY, sides[0], sides[1], sides[2]);



    // We can then apply this mass to our objects body.

    dBodySetMass(Object.Body, &m);



    // Here we create the actual geom object using dCreateBox. Note that this also adds the geom to our

       // collision space and sets the size of the geom to that of our box mass.

    Object.Geom[0] = dCreateBox(Space, sides[0], sides[1], sides[2]);

    // And lastly we want to associate the body with the geom using dGeomSetBody. Setting a body on a geom automatically

       // combines the position vector and rotation matrix of the body and geom so that setting the position or orientation

       // of one will set the value for both objects. The ODE docs have a lot more to say about the geom functions.

    dGeomSetBody(Object.Geom[0], Object.Body);

}

void CloseODE()

{

    // Destroy all joints in our joint group

    dJointGroupDestroy(contactgroup);



    // Destroy the collision space. When a space is destroyed, and its cleanup mode is 1 (the default)

       // then all the geoms in that space are automatically destroyed as well.

    dSpaceDestroy(Space);

    // Destroy the world and everything in it. This includes all bodies and all joints that are not part of a joint group.

    dWorldDestroy(World);

}

void ODEtoOGL(float* M, const float* p, const float* R)

{

    M[0]  = R[0]; M[1]  = R[4]; M[2]  = R[8];  M[3]  = 0;

    M[4]  = R[1]; M[5]  = R[5]; M[6]  = R[9];  M[7]  = 0;

    M[8]  = R[2]; M[9]  = R[6]; M[10] = R[10]; M[11] = 0;

    M[12] = p[0]; M[13] = p[1]; M[14] = p[2];  M[15] = 1;

}

void DrawBox(const float sides[3], const float pos[3], const float R[12])

{

    float mat_ambient[] = { 0.8, 0.8, 0.8, 1.0 };

    float mat_diffuse[] = { 0.8, 0.8, 0.8, 1.0 };

    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);

    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);



    glBindTexture(GL_TEXTURE_2D, texture[0].TexID);



    glPushMatrix();

    GeomMatrix.ODEtoOGL(pos, R);

    glMultMatrixf(GeomMatrix.Element);

    glBegin(GL_TRIANGLES);

        // Front Face

            glNormal3fv(&polygon[0].Vertex[0].nx);

        glTexCoord2f(0.0f, 0.0f); glVertex3fv(&polygon[0].Vertex[0].x);

        glTexCoord2f(1.0f, 0.0f); glVertex3fv(&polygon[0].Vertex[1].x);

        glTexCoord2f(1.0f, 1.0f); glVertex3fv(&polygon[0].Vertex[2].x);



        glTexCoord2f(0.0f, 0.0f); glVertex3fv(&polygon[1].Vertex[0].x);

        glTexCoord2f(1.0f, 1.0f); glVertex3fv(&polygon[1].Vertex[1].x);

        glTexCoord2f(0.0f, 1.0f); glVertex3fv(&polygon[1].Vertex[2].x);

        // Back Face

            glNormal3fv(&polygon[2].Vertex[0].nx);

        glTexCoord2f(0.0f, 0.0f); glVertex3fv(&polygon[2].Vertex[0].x);

        glTexCoord2f(1.0f, 0.0f); glVertex3fv(&polygon[2].Vertex[1].x);

        glTexCoord2f(1.0f, 1.0f); glVertex3fv(&polygon[2].Vertex[2].x);



        glTexCoord2f(0.0f, 0.0f); glVertex3fv(&polygon[3].Vertex[0].x);

        glTexCoord2f(1.0f, 1.0f); glVertex3fv(&polygon[3].Vertex[1].x);

        glTexCoord2f(0.0f, 1.0f); glVertex3fv(&polygon[3].Vertex[2].x);

        // Top Face

            glNormal3fv(&polygon[4].Vertex[0].nx);

        glTexCoord2f(0.0f, 0.0f); glVertex3fv(&polygon[4].Vertex[0].x);

        glTexCoord2f(1.0f, 0.0f); glVertex3fv(&polygon[4].Vertex[1].x);

        glTexCoord2f(1.0f, 1.0f); glVertex3fv(&polygon[4].Vertex[2].x);



        glTexCoord2f(0.0f, 0.0f); glVertex3fv(&polygon[5].Vertex[0].x);

        glTexCoord2f(1.0f, 1.0f); glVertex3fv(&polygon[5].Vertex[1].x);

        glTexCoord2f(0.0f, 1.0f); glVertex3fv(&polygon[5].Vertex[2].x);

        // Bottom Face

            glNormal3fv(&polygon[6].Vertex[0].nx);

        glTexCoord2f(0.0f, 0.0f); glVertex3fv(&polygon[6].Vertex[0].x);

        glTexCoord2f(1.0f, 0.0f); glVertex3fv(&polygon[6].Vertex[1].x);

        glTexCoord2f(1.0f, 1.0f); glVertex3fv(&polygon[6].Vertex[2].x);



        glTexCoord2f(0.0f, 0.0f); glVertex3fv(&polygon[7].Vertex[0].x);

        glTexCoord2f(1.0f, 1.0f); glVertex3fv(&polygon[7].Vertex[1].x);

        glTexCoord2f(0.0f, 1.0f); glVertex3fv(&polygon[7].Vertex[2].x);

        // Right face

            glNormal3fv(&polygon[8].Vertex[0].nx);

        glTexCoord2f(0.0f, 0.0f); glVertex3fv(&polygon[8].Vertex[0].x);

        glTexCoord2f(1.0f, 0.0f); glVertex3fv(&polygon[8].Vertex[1].x);

        glTexCoord2f(1.0f, 1.0f); glVertex3fv(&polygon[8].Vertex[2].x);



        glTexCoord2f(0.0f, 0.0f); glVertex3fv(&polygon[9].Vertex[0].x);

        glTexCoord2f(1.0f, 1.0f); glVertex3fv(&polygon[9].Vertex[1].x);

        glTexCoord2f(0.0f, 1.0f); glVertex3fv(&polygon[9].Vertex[2].x);

        // Left Face

            glNormal3fv(&polygon[10].Vertex[0].nx);

        glTexCoord2f(0.0f, 0.0f); glVertex3fv(&polygon[10].Vertex[0].x);

        glTexCoord2f(1.0f, 0.0f); glVertex3fv(&polygon[10].Vertex[1].x);

        glTexCoord2f(1.0f, 1.0f); glVertex3fv(&polygon[10].Vertex[2].x);



        glTexCoord2f(0.0f, 0.0f); glVertex3fv(&polygon[11].Vertex[0].x);

        glTexCoord2f(1.0f, 1.0f); glVertex3fv(&polygon[11].Vertex[1].x);

        glTexCoord2f(0.0f, 1.0f); glVertex3fv(&polygon[11].Vertex[2].x);

    glEnd();

    glPopMatrix();

}


void DrawGeom (dGeomID g, const dReal *pos, const dReal *R, int show_aabb)

{

    // If the geom ID is missing then return immediately.

    if (!g)

        return;



    // If there was no position vector supplied then get the existing position.

    if (!pos)

        pos = dGeomGetPosition (g);



    // If there was no rotation matrix given then get the existing rotation.

    if (!R)

        R = dGeomGetRotation (g);



    // Get the geom's class type.

    int type = dGeomGetClass (g);



    if (type == dBoxClass)

    {

        // Create a temporary array of floats to hold the box dimensions.

        dReal sides[3];

        dGeomBoxGetLengths(g, sides);



        // Now to actually render the box we make a call to DrawBox, passing the geoms dimensions, position vector and

           // rotation matrix. And this function is the subject of our next discussion.

        DrawBox(sides, pos, R);

    }

}

static void nearCallback (void *data, dGeomID o1, dGeomID o2)

{

    // Temporary index for each contact

    int i;



   // Get the dynamics body for each geom

    dBodyID b1 = dGeomGetBody(o1);

    dBodyID b2 = dGeomGetBody(o2);



    // Create an array of dContact objects to hold the contact joints

    dContact contact[MAX_CONTACTS];



    // Now we set the joint properties of each contact. Going into the full details here would require a tutorial of its

       // own. I'll just say that the members of the dContact structure control the joint behaviour, such as friction,

       // velocity and bounciness. See section 7.3.7 of the ODE manual and have fun experimenting to learn more.

    for (i = 0; i < MAX_CONTACTS; i++)

    {

        contact[i].surface.mode = dContactBounce | dContactSoftCFM;

        contact[i].surface.mu = dInfinity;

        contact[i].surface.mu2 = 0;

        contact[i].surface.bounce = 0.01;

        contact[i].surface.bounce_vel = 0.1;

        contact[i].surface.soft_cfm = 0.01;

    }



    // Here we do the actual collision test by calling dCollide. It returns the number of actual contact points or zero

       // if there were none. As well as the geom IDs, max number of contacts we also pass the address of a dContactGeom

       // as the fourth parameter. dContactGeom is a substructure of a dContact object so we simply pass the address of

       // the first dContactGeom from our array of dContact objects and then pass the offset to the next dContactGeom

       // as the fifth paramater, which is the size of a dContact structure. That made sense didn't it?

    if (int numc = dCollide(o1, o2, MAX_CONTACTS, &contact[0].geom, sizeof(dContact)))

    {

        // To add each contact point found to our joint group we call dJointCreateContact which is just one of the many

           // different joint types available.

        for (i = 0; i < numc; i++)

        {

            // dJointCreateContact needs to know which world and joint group to work with as well as the dContact

               // object itself. It returns a new dJointID which we then use with dJointAttach to finally create the

               // temporary contact joint between the two geom bodies.

            dJointID c = dJointCreateContact(World, contactgroup, contact + i);

            dJointAttach(c, b1, b2);

        }

    }

}



void SimLoop()

{

    // dSpaceCollide determines which pairs of geoms in the space we pass to it may potentially intersect. We must also

       // pass the address of a callback function that we will provide. The callback function is responsible for

       // determining which of the potential intersections are actual collisions before adding the collision joints to our

       // joint group called contactgroup, this gives us the chance to set the behaviour of these joints before adding them

       // to the group. The second parameter is a pointer to any data that we may want to pass to our callback routine.

       // We will cover the details of the nearCallback routine in the next section.

    dSpaceCollide(Space, 0, &nearCallback);



    // Now we advance the simulation by calling dWorldQuickStep. This is a faster version of dWorldStep but it is also

       // slightly less accurate. As well as the World object ID we also pass a step size value. In each step the simulation

       // is updated by a certain number of smaller steps or iterations. The default number of iterations is 20 but you can

       // change this by calling dWorldSetQuickStepNumIterations.

    dWorldQuickStep(World, 0.05);



    // Remove all temporary collision joints now that the world has been stepped

    dJointGroupEmpty(contactgroup);



    // And we finish by calling DrawGeom which renders the objects according to their type or class

    DrawGeom(Object.Geom[0], 0, 0, 0);

}

