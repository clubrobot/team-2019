#ifndef __MATRIX_H
#define __MATRIX_H

#include <Arduino.h>
#include "Scara.h"

typedef double** matrix_t;

class Matrix
{
	
	public:
		matrix_t createMatrix22(double X1, double X2, double Y1, double Y2);
		matrix_t createMatrix21(double X1, double Y1);

		matrix_t multMatrix22x12(matrix_t mat22, matrix_t mat12);

		double norm(matrix_t mat);
		double det(matrix_t mat);
		matrix_t solve(matrix_t mat22, matrix_t mat12);
};

#endif /* __MATRIX_H */