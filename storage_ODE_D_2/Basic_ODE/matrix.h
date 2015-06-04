// Matrix Class    by Alan Baylis 2001

#ifndef _Matrix_H
#define _Matrix_H

#include "vector.h"
#include "quat.h"

class MATRIX
{
    public:
        MATRIX();
        ~MATRIX();

        void LoadIdentity();
        void ODEtoOGL(const double* p, const double* R);
        void CopyMatrix(float m[16]);
        void MultMatrix(float m[16]);
        void MatrixInverse(); 
        void MatrixFromAxisAngle(VECTOR axis, float theta);
        void QuatToMatrix(QUAT quat); 

        float Element[16];
};

#endif

