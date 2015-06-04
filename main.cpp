#include <ode/ode.h>
#include <drawstuff/drawstuff.h>

#define ITERS 20		// number of iterations

///////////////////////////////
//
// bodies a.o.
//
 ///////////////////////////////
// dynamics and collision objects
static dWorldID world;
static dSpaceID space;
static dBodyID body;
static dGeomID geom;
static dMass m;
static dJointGroupID contactgroup;

static dGeomID ground;

///////////////////////////////
//
// vars
//
 ///////////////////////////////
int counter = 0;

static int bodies;
static int joints;
static int boxes;
static int spheres;

static int wb_stepsdis[10000];

static int wb;

static bool doFast;

///////////////////////////////
//
// functions
//
 ///////////////////////////////
// this is called by dSpaceCollide when two objects in space are
// potentially colliding.
static void nearCallback (void *data, dGeomID o1, dGeomID o2)
{
    dBodyID b1 = dGeomGetBody(o1);
    dBodyID b2 = dGeomGetBody(o2);
    dContact contact;
    contact.surface.mode = dContactBounce | dContactSoftCFM;
    // friction parameter
    contact.surface.mu = dInfinity;
    // bounce is the amount of "bouncyness".
    contact.surface.bounce = 0.9;
    // bounce_vel is the minimum incoming velocity to cause a bounce
    contact.surface.bounce_vel = 0.1;
    // constraint force mixing parameter
    contact.surface.soft_cfm = 0.001;
    if (int numc = dCollide (o1,o2,1,&contact.geom,sizeof(dContact))) {
        dJointID c = dJointCreateContact (world,contactgroup,&contact);
        dJointAttach (c,b1,b2);
    }
}

// start simulation - set viewpoint
static void start()
{

	dAllocateODEDataForThread(dAllocateMaskAll);

	printf("[%s:%d] dAllocateODEDataForThread => done\n", __FILE__, __LINE__);


    static float xyz[3] = {2.0f,-2.0f,1.7600f};
    static float hpr[3] = {140.000f,-17.0000f,0.0000f};
    dsSetViewpoint (xyz,hpr);

    //debug
    printf("[%s:%d] DS_WOOD => %d\n", __FILE__, __LINE__, DS_WOOD);



	printf ("Press:\t'a' to increase speed.\n"
			"\t'z' to decrease speed.\n"
			"\t',' to steer left.\n"
			"\t'.' to steer right.\n"
			"\t' ' to reset speed and steering.\n"
			"\t'[' to turn the cannon left.\n"
			"\t']' to turn the cannon right.\n"
			"\t'1' to raise the cannon.\n"
			"\t'2' to lower the cannon.\n"
			"\t'x' to shoot from the cannon.\n"
			"\t'f' to toggle fast step mode.\n"
			"\t'+' to increase AutoEnableDepth.\n"
			"\t'-' to decrease AutoEnableDepth.\n"
			"\t'r' to reset simulation.\n");


}

// simulation loop
static void simLoop (int pause) {

//	dsSetTexture (2);
	dsSetTexture (DS_WOOD);

//#ifdef BOX
//
//	printf("[%s:%d] BOX => defined\n", __FILE__, __LINE__);
//
//#else
//
//	printf("[%s:%d] BOX => NOT defined\n", __FILE__, __LINE__);
//
//#endif

    const dReal *pos;
    const dReal *R;
    // find collisions and add contact joints
    dSpaceCollide (space,0,&nearCallback);
    // step the simulation
    dWorldQuickStep (world,0.01);
    // remove all contact joints
    dJointGroupEmpty (contactgroup);
    // redraw sphere at new location
    pos = dGeomGetPosition (geom);
    R = dGeomGetRotation (geom);

    dsDrawSphere ((float*)pos,(float*)R,dGeomSphereGetRadius (geom));
//    dsDrawSphere (pos,R,dGeomSphereGetRadius (geom));


    ///////////////////////////////
	//
	// debug
	//
	 ///////////////////////////////
//	if (counter % 10 == 0) {
//
//		printf("[%s:%d] pause => %d\n", __FILE__, __LINE__, pause);
//
//	}

}//static void simLoop (int pause)

void resetSimulation() {
	int i;
	i = 0;
	// destroy world if it exists
	if (bodies)
	{
		dJointGroupDestroy (contactgroup);
		dSpaceDestroy (space);
		dWorldDestroy (world);
	}

	for (i = 0; i < 1000; i++)
		wb_stepsdis[i] = 0;

	// recreate world

	world = dWorldCreate();

//	space = dHashSpaceCreate( 0 );
//	space = dSimpleSpaceCreate( 0 );
	space = dSweepAndPruneSpaceCreate( 0, dSAP_AXES_XYZ );

	contactgroup = dJointGroupCreate (0);
	dWorldSetGravity (world,0,0,-1.5);
	dWorldSetCFM (world, 1e-5);
	dWorldSetERP (world, 0.8);
	dWorldSetQuickStepNumIterations (world,ITERS);
	ground = dCreatePlane (space,0,0,1,0);

	bodies = 0;
	joints = 0;
	boxes = 0;
	spheres = 0;
	wb = 0;

#ifdef CARS
	for (dReal x = 0.0; x < COLS*(LENGTH+RADIUS); x += LENGTH+RADIUS)
		for (dReal y = -((ROWS-1)*(WIDTH/2+RADIUS)); y <= ((ROWS-1)*(WIDTH/2+RADIUS)); y += WIDTH+RADIUS*2)
			makeCar(x, y, bodies, joints, boxes, spheres);
#endif
#ifdef WALL
	bool offset = false;
	for (dReal z = WBOXSIZE/2.0; z <= WALLHEIGHT; z+=WBOXSIZE)
	{
		offset = !offset;
		for (dReal y = (-WALLWIDTH+z)/2; y <= (WALLWIDTH-z)/2; y+=WBOXSIZE)
		{
			wall_bodies[wb] = dBodyCreate (world);
			dBodySetPosition (wall_bodies[wb],-20,y,z);
			dMassSetBox (&m,1,WBOXSIZE,WBOXSIZE,WBOXSIZE);
			dMassAdjust (&m, WALLMASS);
			dBodySetMass (wall_bodies[wb],&m);
			wall_boxes[wb] = dCreateBox (space,WBOXSIZE,WBOXSIZE,WBOXSIZE);
			dGeomSetBody (wall_boxes[wb],wall_bodies[wb]);
			//dBodyDisable(wall_bodies[wb++]);
			wb++;
		}
	}
	dMessage(0,"wall boxes: %i", wb);
#endif
#ifdef BALLS
	for (dReal x = -7; x <= -4; x+=1)
		for (dReal y = -1.5; y <= 1.5; y+=1)
			for (dReal z = 1; z <= 4; z+=1)
			{
				b = dBodyCreate (world);
				dBodySetPosition (b,x*RADIUS*2,y*RADIUS*2,z*RADIUS*2);
				dMassSetSphere (&m,1,RADIUS);
				dMassAdjust (&m, BALLMASS);
				dBodySetMass (b,&m);
				sphere[spheres] = dCreateSphere (space,RADIUS);
				dGeomSetBody (sphere[spheres++],b);
			}
#endif
#ifdef ONEBALL
	b = dBodyCreate (world);
	dBodySetPosition (b,0,0,2);
	dMassSetSphere (&m,1,RADIUS);
	dMassAdjust (&m, 1);
	dBodySetMass (b,&m);
	sphere[spheres] = dCreateSphere (space,RADIUS);
	dGeomSetBody (sphere[spheres++],b);
#endif
#ifdef BALLSTACK
	for (dReal z = 1; z <= 6; z+=1)
	{
		b = dBodyCreate (world);
		dBodySetPosition (b,0,0,z*RADIUS*2);
		dMassSetSphere (&m,1,RADIUS);
		dMassAdjust (&m, 0.1);
		dBodySetMass (b,&m);
		sphere[spheres] = dCreateSphere (space,RADIUS);
		dGeomSetBody (sphere[spheres++],b);
	}
#endif
#ifdef CENTIPEDE
	dBodyID lastb = 0;
	for (dReal y = 0; y < 10*LENGTH; y+=LENGTH+0.1)
	{
		// chassis body

		b = body[bodies] = dBodyCreate (world);
		dBodySetPosition (body[bodies],-15,y,STARTZ);
		dMassSetBox (&m,1,WIDTH,LENGTH,HEIGHT);
		dMassAdjust (&m,CMASS);
		dBodySetMass (body[bodies],&m);
		box[boxes] = dCreateBox (space,WIDTH,LENGTH,HEIGHT);
		dGeomSetBody (box[boxes++],body[bodies++]);

		for (dReal x = -17; x > -20; x-=RADIUS*2)
		{
			body[bodies] = dBodyCreate (world);
			dBodySetPosition(body[bodies], x, y, STARTZ);
			dMassSetSphere(&m, 1, RADIUS);
			dMassAdjust(&m, WMASS);
			dBodySetMass(body[bodies], &m);
			sphere[spheres] = dCreateSphere (space, RADIUS);
			dGeomSetBody (sphere[spheres++], body[bodies]);

			joint[joints] = dJointCreateHinge2 (world,0);
			if (x == -17)
				dJointAttach (joint[joints],b,body[bodies]);
			else
				dJointAttach (joint[joints],body[bodies-2],body[bodies]);
			const dReal *a = dBodyGetPosition (body[bodies++]);
			dJointSetHinge2Anchor (joint[joints],a[0],a[1],a[2]);
			dJointSetHinge2Axis1 (joint[joints],0,0,1);
			dJointSetHinge2Axis2 (joint[joints],1,0,0);
			dJointSetHinge2Param (joint[joints],dParamSuspensionERP,1.0);
			dJointSetHinge2Param (joint[joints],dParamSuspensionCFM,1e-5);
			dJointSetHinge2Param (joint[joints],dParamLoStop,0);
			dJointSetHinge2Param (joint[joints],dParamHiStop,0);
			dJointSetHinge2Param (joint[joints],dParamVel2,-10.0);
			dJointSetHinge2Param (joint[joints++],dParamFMax2,FMAX);

			body[bodies] = dBodyCreate (world);
			dBodySetPosition(body[bodies], -30 - x, y, STARTZ);
			dMassSetSphere(&m, 1, RADIUS);
			dMassAdjust(&m, WMASS);
			dBodySetMass(body[bodies], &m);
			sphere[spheres] = dCreateSphere (space, RADIUS);
			dGeomSetBody (sphere[spheres++], body[bodies]);

			joint[joints] = dJointCreateHinge2 (world,0);
			if (x == -17)
				dJointAttach (joint[joints],b,body[bodies]);
			else
				dJointAttach (joint[joints],body[bodies-2],body[bodies]);
			const dReal *b = dBodyGetPosition (body[bodies++]);
			dJointSetHinge2Anchor (joint[joints],b[0],b[1],b[2]);
			dJointSetHinge2Axis1 (joint[joints],0,0,1);
			dJointSetHinge2Axis2 (joint[joints],1,0,0);
			dJointSetHinge2Param (joint[joints],dParamSuspensionERP,1.0);
			dJointSetHinge2Param (joint[joints],dParamSuspensionCFM,1e-5);
			dJointSetHinge2Param (joint[joints],dParamLoStop,0);
			dJointSetHinge2Param (joint[joints],dParamHiStop,0);
			dJointSetHinge2Param (joint[joints],dParamVel2,10.0);
			dJointSetHinge2Param (joint[joints++],dParamFMax2,FMAX);
		}
		if (lastb)
		{
			dJointID j = dJointCreateFixed(world,0);
			dJointAttach (j, b, lastb);
			dJointSetFixed(j);
		}
		lastb = b;
	}
#endif
#ifdef BOX
	body[bodies] = dBodyCreate (world);
	dBodySetPosition (body[bodies],0,0,HEIGHT/2);
	dMassSetBox (&m,1,LENGTH,WIDTH,HEIGHT);
	dMassAdjust (&m, 1);
	dBodySetMass (body[bodies],&m);
	box[boxes] = dCreateBox (space,LENGTH,WIDTH,HEIGHT);
	dGeomSetBody (box[boxes++],body[bodies++]);
#endif
#ifdef CANNON
	cannon_ball_body = dBodyCreate (world);
	cannon_ball_geom = dCreateSphere (space,CANNON_BALL_RADIUS);
	dMassSetSphereTotal (&m,CANNON_BALL_MASS,CANNON_BALL_RADIUS);
	dBodySetMass (cannon_ball_body,&m);
	dGeomSetBody (cannon_ball_geom,cannon_ball_body);
	dBodySetPosition (cannon_ball_body,CANNON_X,CANNON_Y,CANNON_BALL_RADIUS);
#endif

}//void resetSimulation()


int main (int argc, char **argv)
{

	doFast = true;

    // setup pointers to drawstuff callback functions
    dsFunctions fn;
    fn.version = DS_VERSION;
    fn.start = &start;
    fn.step = &simLoop;
    fn.stop = 0;
    fn.command = 0;
    fn.path_to_textures = "../../drawstuff/textures";

    printf("[%s:%d] version => %d\n", __FILE__, __LINE__, fn.version);
    	//=> [main.cpp:80] version => 2
//    printf("[%s:%d] version => %s\n", __FILE__, __LINE__, fn.version);	//=> stops

    printf("[%s:%d] fn.start => %p\n", __FILE__, __LINE__, fn.start);
    	//=> [main.cpp:84] fn.start => 004016E7

//    dInitODE ();
    dInitODE2(0);

    printf("[%s:%d] dInitODE2(0) => done\n", __FILE__, __LINE__);

	bodies = 0;
	joints = 0;
	boxes = 0;
	spheres = 0;

	resetSimulation();

	printf("[%s:%d] resetSimulation() => done\n", __FILE__, __LINE__);

	///////////////////////////////
	//
	// original codes
	//
	 ///////////////////////////////
    // create world
    world = dWorldCreate ();
    space = dHashSpaceCreate (0);
    dWorldSetGravity (world,0,0,-0.2);
    dWorldSetCFM (world,1e-5);
    dCreatePlane (space,0,0,1,0);
    contactgroup = dJointGroupCreate (0);
    // create object
    body = dBodyCreate (world);
    geom = dCreateSphere (space,0.5);
    dMassSetSphere (&m,1,0.5);
    dBodySetMass (body,&m);
    dGeomSetBody (geom,body);
    // set initial position
    dBodySetPosition (body,0,0,3);

    printf("[%s:%d] starting the simulation...\n", __FILE__, __LINE__);


    // run simulation
    dsSimulationLoop (argc,argv,352,288,&fn);

    // clean up
    dJointGroupDestroy (contactgroup);
    dSpaceDestroy (space);
    dWorldDestroy (world);
    dCloseODE();
    return 0;
}
