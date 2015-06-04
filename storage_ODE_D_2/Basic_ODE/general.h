#ifndef GENERAL_H
#define GENERAL_H

#include "shared.h"
#include "polygon.h"
#include "texture.h"
#include "camera.h"
#include "light.h"

void SetGLLighting(LIGHT *);
void SetGLCamera(CAMERA *);
void SetGLWorld(POLYGON *);
void SetGLProperties();
void SetGLProjection(int Width, int Height);
void SetGLView(int Width, int Height);
void SetGLMaterial();
void SetGLTexture(TEXTURE *);
void DrawGrid();
void DrawBox(const double sides[3], const double pos[3], const double R[12]);
void DrawSphere();
void DrawLightSphere(LIGHT *);
void DrawCone();

#endif //GENERAL_H
