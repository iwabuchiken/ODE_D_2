#include <windows.h>
#include "polygon.h"


POLYGON::POLYGON()
{
}

POLYGON::~POLYGON()
{
}

VECTOR POLYGON::GetNormal()
{
        VECTOR temp; 
        float ux;
        float uy;
        float uz;
        float vx;
        float vy;
        float vz;
          ux = Vertex[1].x - Vertex[0].x;
          uy = Vertex[1].y - Vertex[0].y;
          uz = Vertex[1].z - Vertex[0].z;
          vx = Vertex[2].x - Vertex[0].x;
          vy = Vertex[2].y - Vertex[0].y;
          vz = Vertex[2].z - Vertex[0].z;
          temp.x = (uy*vz)-(vy*uz);
          temp.y = (uz*vx)-(vz*ux);
          temp.z = (ux*vy)-(vx*uy);
        return temp;
}

void POLYGON::SetNormal()
{
        float ux;
        float uy;
        float uz;
        float vx;
        float vy;
        float vz;
          ux = Vertex[1].x - Vertex[0].x;
          uy = Vertex[1].y - Vertex[0].y;
          uz = Vertex[1].z - Vertex[0].z;
          vx = Vertex[2].x - Vertex[0].x;
          vy = Vertex[2].y - Vertex[0].y;
          vz = Vertex[2].z - Vertex[0].z;
          Vertex[0].nx = (uy*vz)-(vy*uz);
          Vertex[0].ny = (uz*vx)-(vz*ux);
          Vertex[0].nz = (ux*vy)-(vx*uy);
        Vertex[1].nx = Vertex[0].nx;
          Vertex[1].ny = Vertex[0].ny;
          Vertex[1].nz = Vertex[0].nz;
          Vertex[2].nx = Vertex[0].nx;
          Vertex[2].ny = Vertex[0].ny;
          Vertex[2].nz = Vertex[0].nz;
}
