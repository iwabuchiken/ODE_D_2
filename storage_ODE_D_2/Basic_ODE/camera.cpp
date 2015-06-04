#include <windows.h>
#include "camera.h"
#include "shared.h"


CAMERA::CAMERA()
{
}

CAMERA::~CAMERA()
{
}

void CAMERA::Reset()
{
    Orientation.Reset();
    Position.Reset();
    Delta_x = 0.0;
    Delta_y = 0.0;
    Delta_z = 0.0;
    Matrix.LoadIdentity();
}

void CAMERA::Update()
{
    Rotate();

    UpdatePosition();
}

void CAMERA::Apply()
{
    Matrix.QuatToMatrix(Orientation);
    Matrix.MatrixInverse();

    glLoadMatrixf(Matrix.Element);
    glTranslatef(-Position.x,-Position.y,-Position.z);
}
