#include <Arduino.h>
#include <math.h>
#include <cmath>
#include <tuple>
#include <iostream>
#include <string>
#include <stdexcept>
#include "Joint.h"
#include "3dof.h"
#include "Matrix.h"

Matrix::Matrix()
{
    
}

Matrix::~Matrix()
{

}

matrix_t Matrix::createMatrix33(double X1, double X2, double X3, double Y1, double Y2, double Y3, double Z1, double Z2, double Z3)
{
    double** matrix = 0;
    matrix = new double*[3];
    matrix[0] = new double[3];
    matrix[1] = new double[3];
    matrix[2] = new double[3];

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

matrix_t Matrix::createMatrix31(double X1, double Y1, double Z1)
{
    double** matrix = 0;
    matrix = new double*[3];
    matrix[0] = new double[1];
    matrix[1] = new double[1];
    matrix[2] = new double[1];

    matrix[0][0] = X1;
    matrix[1][0] = Y1;
    matrix[2][0] = Z1;

    return matrix;
}

matrix_t Matrix::multMatrix33x13(matrix_t mat33, matrix_t mat13)
{
	double a,b,c;

	a = (mat33[0][0] * mat13[0][0]) + (mat33[0][1] * mat13[1][0]) + (mat33[0][2] * mat13[2][0]) ;
	b = (mat33[1][0] * mat13[0][0]) + (mat33[1][1] * mat13[1][0]) + (mat33[1][2] * mat13[2][0]) ;
    c = (mat33[2][0] * mat13[0][0]) + (mat33[2][1] * mat13[1][0]) + (mat33[2][2] * mat13[2][0]) ;

	return createMatrix31(a,b,c);
}

double Matrix::norm(matrix_t mat)
{
    double norm = 0;

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

double Matrix::det(matrix_t mat)
{
    double a,b,c;

    a = mat[0][0]*((mat[1][1]*mat[2][2])-(mat[2][1]*mat[1][2]));
    b = mat[1][0]*((mat[0][1]*mat[2][2])-(mat[2][1]*mat[0][2]));
    c = mat[2][0]*((mat[0][1]*mat[1][2])-(mat[1][1]*mat[0][2]));
    return (a - b + c);
}

matrix_t Matrix::solve(matrix_t mat33, matrix_t mat13)
{
    matrix_t result;
    matrix_t mata = createMatrix33(mat13[0][0],mat13[1][0],mat13[2][0],mat33[1][0],mat33[1][1],mat33[1][2],mat33[2][0],mat33[2][1],mat33[2][2]);
    matrix_t matb = createMatrix33(mat33[0][0],mat33[0][1],mat33[0][2],mat13[0][0],mat13[1][0],mat13[2][0],mat33[2][0],mat33[2][1],mat33[2][2]);
    matrix_t matc = createMatrix33(mat33[0][0],mat33[0][1],mat33[0][2],mat33[1][0],mat33[1][1],mat33[1][2],mat13[0][0],mat13[1][0],mat13[2][0]);

    double a,b,c,deter,detera,deterb,deterc;

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