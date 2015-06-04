// Polygon Class    by Alan Baylis 2001

#ifndef PolygonH
#define PolygonH

#include "texture.h"
#include "vector.h"
#include "vertex.h"

class POLYGON
{
    public:
        POLYGON();
          ~POLYGON();

                VECTOR GetNormal();
                void SetNormal();

          TEXTURE Texture;
          VERTEX Vertex[3];
};

#endif
