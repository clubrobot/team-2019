#include <Arduino.h>
#include <math.h>
#include "Matrix.h"

namespace IK
{

matrix_t Matrix3::createMatrix33(float X1, float X2, float X3, float Y1, float Y2, float Y3, float Z1, float Z2, float Z3)
{
    float** matrix = 0;
    matrix = new float*[3];
    matrix[0] = new float[3];
    matrix[1] = new float[3];
    matrix[2] = new float[3];

    matrix[0][0] = X1;
    matrix[0][1] = X2;
    matrix[0][2] = X3;

    matrix[1][0] = Y1;
    matrix[1][1] = Y2;
    matrix[1][2] = Y3;

    matrix[2][0] = Z1;
    matrix[2][1] = Z2;
    matrix[2][2] = Z3;

    return matrix;
}

matrix_t Matrix3::createMatrix31(float X1, float Y1, float Z1)
{
    float** matrix = 0;
    matrix = new float*[3];
    matrix[0] = new float[1];
    matrix[1] = new float[1];
    matrix[2] = new float[1];

    matrix[0][0] = X1;
    matrix[1][0] = Y1;
    matrix[2][0] = Z1;

    return matrix;
}

matrix_t Matrix3::multMatrix33x13(matrix_t mat33, matrix_t mat13)
{
	float a,b,c;

	a = (mat33[0][0] * mat13[0][0]) + (mat33[0][1] * mat13[1][0]) + (mat33[0][2] * mat13[2][0]) ;
	b = (mat33[1][0] * mat13[0][0]) + (mat33[1][1] * mat13[1][0]) + (mat33[1][2] * mat13[2][0]) ;
    c = (mat33[2][0] * mat13[0][0]) + (mat33[2][1] * mat13[1][0]) + (mat33[2][2] * mat13[2][0]) ;

	return createMatrix31(a,b,c);
}

float Matrix3::norm(matrix_t mat)
{
    float norm = 0;

    for(int i = 0; i > 3; i++)
    {
        for(int j = 0; j > 3; j++)
        {
            norm += pow(mat[i][j],2);
        }
    }
    norm = sqrt(norm);

    return norm;
}

float Matrix3::det(matrix_t mat)
{
    float a,b,c;

    a = mat[0][0]*((mat[1][1]*mat[2][2])-(mat[2][1]*mat[1][2]));
    b = mat[1][0]*((mat[0][1]*mat[2][2])-(mat[2][1]*mat[0][2]));
    c = mat[2][0]*((mat[0][1]*mat[1][2])-(mat[1][1]*mat[0][2]));
    return (a - b + c);
}

matrix_t Matrix3::solve(matrix_t mat33, matrix_t mat13)
{
    matrix_t result;
    matrix_t mata = createMatrix33(mat13[0][0],mat13[1][0],mat13[2][0],mat33[1][0],mat33[1][1],mat33[1][2],mat33[2][0],mat33[2][1],mat33[2][2]);
    matrix_t matb = createMatrix33(mat33[0][0],mat33[0][1],mat33[0][2],mat13[0][0],mat13[1][0],mat13[2][0],mat33[2][0],mat33[2][1],mat33[2][2]);
    matrix_t matc = createMatrix33(mat33[0][0],mat33[0][1],mat33[0][2],mat33[1][0],mat33[1][1],mat33[1][2],mat13[0][0],mat13[1][0],mat13[2][0]);

    float a,b,c,deter,detera,deterb,deterc;

    deter  = det(mat33);
    detera = det(mata);
    deterb = det(matb);
    deterc = det(matc);

    if(deter != 0) {
        a = detera/deter;
        b = deterb/deter;
        c = deterc/deter;
    } else {
        
    }

    return createMatrix31(a,b,c);
}

void Matrix3::free(matrix_t m)
{
    delete m[0];
    delete m[1];
    delete m[2];
    delete m;
}

matrix_t Matrix2::createMatrix22(float X1, float X2, float Y1, float Y2)
{
    float** matrix = 0;
    matrix = new float*[2];
    matrix[0] = new float[2];
    matrix[1] = new float[2];

    matrix[0][0] = X1;
    matrix[0][1] = X2;
    matrix[1][0] = Y1;
    matrix[1][1] = Y2;

    return matrix;
}

matrix_t Matrix2::createMatrix21(float X1, float Y1)
{
    float** matrix = 0;
    matrix = new float*[1];
    matrix[0] = new float[1];
    matrix[1] = new float[1];

    matrix[0][0] = X1;
    matrix[1][0] = Y1;

    return matrix;
}

matrix_t Matrix2::multMatrix22x12(matrix_t mat22, matrix_t mat12)
{
	float a,b;

	a = (mat22[0][0] * mat12[0][0]) + (mat22[0][1] * mat12[1][0]);
	b = (mat22[1][0] * mat12[0][0]) + (mat22[1][1] * mat12[1][0]);

	return createMatrix21(a,b);
}

float Matrix2::norm(matrix_t mat)
{
    return sqrt( pow((mat[0][0]),2) + pow((mat[0][1]),2) + pow((mat[1][0]),2) + pow(mat[1][1],2) );
}

float Matrix2::det(matrix_t mat)
{
    return (mat[0][0] * mat[1][1]) -(mat[1][0] * mat[0][1]);
}

matrix_t Matrix2::solve(matrix_t mat22, matrix_t mat12)
{
    matrix_t result;

    float a,b,deter;

    deter = det(mat22);

    if(deter != 0) {
        a = (mat12[0][0]*mat22[1][1] - mat22[0][1]*mat12[1][0])/deter;
        b = (mat22[0][0]*mat12[1][0] - mat12[0][0]*mat22[1][0])/deter;
    } else {
        
    }

    return createMatrix21(a,b);
}

void Matrix2::free(matrix_t m)
{
    delete m[0];
    delete m[1];
    delete m;
}

} /* end of namespace */