#include <windows.h>
#include "shared.h"
#include "polygon.h"
#include "camera.h"
#include "texture.h"
#include "light.h"
#include "ode.h"

extern int numCameras;
extern int numLights;
extern int currentLight;
extern POLYGON* polygon;
extern TEXTURE* texture;
extern MATRIX GeomMatrix;

void SetGLLighting(LIGHT* light)         // Loop through and reset all the lights
{
    int temp;
    for (temp = 0; temp <= numLights; temp++)
    {
        light[temp].LightNumber = temp;
        light[temp].Reset();
    }
    glEnable(GL_LIGHTING);
}

void SetGLCamera(CAMERA* camera)          // Loop through and reset all the cameras
{
    int temp;
    for(temp = 0; temp <= numCameras; temp++)
        camera[temp].Reset();
}

void SetGLWorld(POLYGON* polygon)            // Initialize the world data 
{
//Front
    polygon[0].Vertex[0].x = -1.0;
    polygon[0].Vertex[0].y = -1.0;
    polygon[0].Vertex[0].z = 1.0;
    polygon[0].Vertex[1].x = 1.0;
    polygon[0].Vertex[1].y = -1.0;
    polygon[0].Vertex[1].z = 1.0;
    polygon[0].Vertex[2].x = 1.0;
    polygon[0].Vertex[2].y = 1.0;
    polygon[0].Vertex[2].z = 1.0;

    polygon[1].Vertex[0].x = -1.0;
    polygon[1].Vertex[0].y = -1.0;
    polygon[1].Vertex[0].z = 1.0;
    polygon[1].Vertex[1].x = 1.0;
    polygon[1].Vertex[1].y = 1.0;
    polygon[1].Vertex[1].z = 1.0;
    polygon[1].Vertex[2].x = -1.0;
    polygon[1].Vertex[2].y = 1.0;
    polygon[1].Vertex[2].z = 1.0;
//Back
    polygon[2].Vertex[0].x = 1.0;
    polygon[2].Vertex[0].y = -1.0;
    polygon[2].Vertex[0].z = -1.0;
    polygon[2].Vertex[1].x = -1.0;
    polygon[2].Vertex[1].y = -1.0;
    polygon[2].Vertex[1].z = -1.0;
    polygon[2].Vertex[2].x = -1.0;
    polygon[2].Vertex[2].y = 1.0;
    polygon[2].Vertex[2].z = -1.0;

    polygon[3].Vertex[0].x = 1.0;
    polygon[3].Vertex[0].y = -1.0;
    polygon[3].Vertex[0].z = -1.0;
    polygon[3].Vertex[1].x = -1.0;
    polygon[3].Vertex[1].y = 1.0;
    polygon[3].Vertex[1].z = -1.0;
    polygon[3].Vertex[2].x = 1.0;
    polygon[3].Vertex[2].y = 1.0;
    polygon[3].Vertex[2].z = -1.0;
//Left
    polygon[4].Vertex[0].x = -1.0;
    polygon[4].Vertex[0].y = -1.0;
    polygon[4].Vertex[0].z = -1.0;
    polygon[4].Vertex[1].x = -1.0;
    polygon[4].Vertex[1].y = -1.0;
    polygon[4].Vertex[1].z = 1.0;
    polygon[4].Vertex[2].x = -1.0;
    polygon[4].Vertex[2].y = 1.0;
    polygon[4].Vertex[2].z = 1.0;

    polygon[5].Vertex[0].x = -1.0;
    polygon[5].Vertex[0].y = -1.0;
    polygon[5].Vertex[0].z = -1.0;
    polygon[5].Vertex[1].x = -1.0;
    polygon[5].Vertex[1].y = 1.0;
    polygon[5].Vertex[1].z = 1.0;
    polygon[5].Vertex[2].x = -1.0;
    polygon[5].Vertex[2].y = 1.0;
    polygon[5].Vertex[2].z = -1.0;
//Right
    polygon[6].Vertex[0].x = 1.0;
    polygon[6].Vertex[0].y = -1.0;
    polygon[6].Vertex[0].z = 1.0;
    polygon[6].Vertex[1].x = 1.0;
    polygon[6].Vertex[1].y = -1.0;
    polygon[6].Vertex[1].z = -1.0;
    polygon[6].Vertex[2].x = 1.0;
    polygon[6].Vertex[2].y = 1.0;
    polygon[6].Vertex[2].z = -1.0;

    polygon[7].Vertex[0].x = 1.0;
    polygon[7].Vertex[0].y = -1.0;
    polygon[7].Vertex[0].z = 1.0;
    polygon[7].Vertex[1].x = 1.0;
    polygon[7].Vertex[1].y = 1.0;
    polygon[7].Vertex[1].z = -1.0;
    polygon[7].Vertex[2].x = 1.0;
    polygon[7].Vertex[2].y = 1.0;
    polygon[7].Vertex[2].z = 1.0;
//Top
    polygon[8].Vertex[0].x = 1.0;
    polygon[8].Vertex[0].y = 1.0;
    polygon[8].Vertex[0].z = 1.0;
    polygon[8].Vertex[1].x = 1.0;
    polygon[8].Vertex[1].y = 1.0;
    polygon[8].Vertex[1].z = -1.0;
    polygon[8].Vertex[2].x = -1.0;
    polygon[8].Vertex[2].y = 1.0;
    polygon[8].Vertex[2].z = -1.0;

    polygon[9].Vertex[0].x = 1.0;
    polygon[9].Vertex[0].y = 1.0;
    polygon[9].Vertex[0].z = 1.0;
    polygon[9].Vertex[1].x = -1.0;
    polygon[9].Vertex[1].y = 1.0;
    polygon[9].Vertex[1].z = -1.0;
    polygon[9].Vertex[2].x = -1.0;
    polygon[9].Vertex[2].y = 1.0;
    polygon[9].Vertex[2].z = 1.0;
//Bottom
    polygon[10].Vertex[0].x = -1.0;
    polygon[10].Vertex[0].y = -1.0;
    polygon[10].Vertex[0].z = 1.0;
    polygon[10].Vertex[1].x = -1.0;
    polygon[10].Vertex[1].y = -1.0;
    polygon[10].Vertex[1].z = -1.0;
    polygon[10].Vertex[2].x = 1.0;
    polygon[10].Vertex[2].y = -1.0;
    polygon[10].Vertex[2].z = -1.0;

    polygon[11].Vertex[0].x = -1.0;
    polygon[11].Vertex[0].y = -1.0;
    polygon[11].Vertex[0].z = 1.0;
    polygon[11].Vertex[1].x = 1.0;
    polygon[11].Vertex[1].y = -1.0;
    polygon[11].Vertex[1].z = -1.0;
    polygon[11].Vertex[2].x = 1.0;
    polygon[11].Vertex[2].y = -1.0;
    polygon[11].Vertex[2].z = 1.0;

    polygon[0].SetNormal();
    polygon[1].SetNormal();
    polygon[2].SetNormal();
    polygon[3].SetNormal();
    polygon[4].SetNormal();
    polygon[5].SetNormal();
    polygon[6].SetNormal();
    polygon[7].SetNormal();
    polygon[8].SetNormal();
    polygon[9].SetNormal();
    polygon[10].SetNormal();
    polygon[11].SetNormal();
}

void SetGLProperties()
{
    glCullFace(GL_BACK);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(1.0);
    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_NORMALIZE);
    glEnable(GL_CULL_FACE);
}

void SetGLProjection(int Width, int Height)
{
    if (Height==0)
        Height=1;
    glViewport(0, 0, Width, Height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0,(float)Width/(float)Height,0.5,200.0);
}

void SetGLView(int Width, int Height)
{
    SetGLProjection(Width, Height);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void SetGLMaterial()
{
    float mat_ambient[] = { 1.0, 1.0, 1.0, 1.0 };
    float mat_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
    float mat_specular[] = { 0.9, 0.9, 0.9, 1.0 };
    float mat_emission[] = { 0.0, 0.0, 0.0, 1.0 };
    float mat_shininess[] = { 80.0 };

    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
}

void SetGLTexture(TEXTURE* texture)            // This function will be rewritten later
{
    sprintf(texture[0].TexName, "%s", "tile.tga");
    texture[0].LoadTGA();
}

void DrawGrid()
{
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);
    glPushMatrix();
    //glTranslatef(0,-2.0,0);
    float Line = -10;
    int Grid;
    glBegin(GL_LINES);
    for(Grid = 0; Grid <= 20; Grid += 1)
    {
        glColor3f(0.0f,1.0f,0.0f);
        glVertex3f(Line + Grid, 0, -10);
        glVertex3f(Line + Grid, 0, 10);
        glVertex3f(-10, 0, Line + Grid);
        glVertex3f(10, 0, Line + Grid);
    }
    glEnd();
    glPopMatrix();
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
}

/* Draw a cube with the geom rotation
 *   - pos[] is the x,y,z of the center of the object.
 *   - R[] is a 3x3 rotation matrix for the object, stored by row like this:
 *        [ R11 R12 R13 0 ]
 *        [ R21 R22 R23 0 ]
 *        [ R31 R32 R33 0 ]
*/
void DrawBox(const double sides[3], const double pos[3], const double R[12])
{
    float mat_ambient[] = { 0.8, 0.8, 0.8, 1.0 };
    float mat_diffuse[] = { 0.8, 0.8, 0.8, 1.0 };
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);

    glBindTexture(GL_TEXTURE_2D, texture[0].TexID);

    glPushMatrix();
    GeomMatrix.ODEtoOGL(pos, R);
    glMultMatrixf(GeomMatrix.Element);
//    glTranslatef(0, 3, -5);
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

void DrawLightSphere(LIGHT* light)
{
    float mat_ambient[] = { 1.0, 0.2, 0.1, 1.0 };
    float mat_diffuse[] = { 1.0, 0.2, 0.1, 1.0 };
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glDisable(GL_TEXTURE_2D);
    glPushMatrix();
    glTranslatef(light[currentLight].Position.x, light[currentLight].Position.y, light[currentLight].Position.z);
    GLUquadricObj * sphere = gluNewQuadric();
    gluQuadricOrientation(sphere, GLU_OUTSIDE);
    gluSphere(sphere,0.3,20,20);
    glPopMatrix();
    glEnable(GL_TEXTURE_2D);
}

void DrawSphere()
{
    float mat_ambient[] = { 0.8, 0.5, 0.1, 1.0 };
    float mat_diffuse[] = { 0.8, 0.5, 0.1, 1.0 };
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);

    glDisable(GL_TEXTURE_2D);
    glPushMatrix();
    glTranslatef(-3.0f,-1.0f,-8.0f);
    GLUquadricObj * sphere = gluNewQuadric();
    gluQuadricOrientation(sphere, GLU_OUTSIDE);
    gluSphere(sphere,1.0,50,50);
    glPopMatrix();
    glEnable(GL_TEXTURE_2D);
}

void DrawCone()
{
    float mat_ambient[] = { 0.1, 0.5, 1.0, 1.0 };
    float mat_diffuse[] = { 0.1, 0.5, 1.0, 1.0 };
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);

    glDisable(GL_TEXTURE_2D);
    glDisable(GL_CULL_FACE);
    glPushMatrix();
    glTranslatef(3.0f,-2.0f,-8.0f);
    glRotatef(-90,1,0,0);
    GLUquadricObj * cylinder = gluNewQuadric();
    gluQuadricOrientation(cylinder, GLU_OUTSIDE);
    gluCylinder(cylinder,1.0,0.0,2.0,20,20);
    glPopMatrix();
    glEnable(GL_CULL_FACE);
    glEnable(GL_TEXTURE_2D);
}
