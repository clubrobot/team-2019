#ifndef __MATRIX_H
#define __MATRIX_H

#include <Arduino.h>
#include "IK.h"


typedef double** matrix_t;

class Matrix
{
	
	public:
		Matrix();

		matrix_t createMatrix33(double X1, double X2, double X3, double Y1, double Y2, double Y3, double Z1, double Z2, double Z3);
		matrix_t createMatrix31(double X1, double Y1, double Z1);

		matrix_t multMatrix33x13(matrix_t mat33, matrix_t mat13);

		double norm(matrix_t mat);
		double det(matrix_t mat);
		matrix_t solve(matrix_t mat33, matrix_t mat13);

		~Matrix();
};

#endif /* __MATRIX_H */
