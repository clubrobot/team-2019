#include <Arduino.h>
#include <math.h>
#include <cmath>
#include <tuple>
#include <iostream>
#include <string>
#include <stdexcept>
#include "Joint.h"
#include "Scara.h"
#include "Matrix.h"

matrix_t Matrix::createMatrix22(double X1, double X2, double Y1, double Y2)
{
    double** matrix = 0;
    matrix = new double*[2];
    matrix[0] = new double[2];
    matrix[1] = new double[2];

    matrix[0][0] = X1;
    matrix[0][1] = X2;
    matrix[1][0] = Y1;
    matrix[1][1] = Y2;

    return matrix;
}

matrix_t Matrix::createMatrix21(double X1, double Y1)
{
    double** matrix = 0;
    matrix = new double*[1];
    matrix[0] = new double[1];
    matrix[1] = new double[1];

    matrix[0][0] = X1;
    matrix[1][0] = Y1;

    return matrix;
}

matrix_t Matrix::multMatrix22x12(matrix_t mat22, matrix_t mat12)
{
	double a,b;

	a = (mat22[0][0] * mat12[0][0]) + (mat22[0][1] * mat12[1][0]);
	b = (mat22[1][0] * mat12[0][0]) + (mat22[1][1] * mat12[1][0]);

	return createMatrix21(a,b);
}