#ifndef __MATRIX_H
#define __MATRIX_H

#include <Arduino.h>


typedef float** matrix_t;

namespace IK
{

class Matrix3
{	
	public:
		matrix_t createMatrix33(float X1, float X2, float X3, float Y1, float Y2, float Y3, float Z1, float Z2, float Z3);
		matrix_t createMatrix31(float X1, float Y1, float Z1);

		matrix_t multMatrix33x13(matrix_t mat33, matrix_t mat13);

		float norm(matrix_t mat);
		float det(matrix_t mat);
		matrix_t solve(matrix_t mat33, matrix_t mat13);

		void free(matrix_t m);
};

class Matrix2
{
	public:
		matrix_t createMatrix22(float X1, float X2, float Y1, float Y2);
		matrix_t createMatrix21(float X1, float Y1);

		matrix_t multMatrix22x12(matrix_t mat22, matrix_t mat12);

		float norm(matrix_t mat);
		float det(matrix_t mat);
		matrix_t solve(matrix_t mat22, matrix_t mat12);
		
		void free(matrix_t m);
};

}



#endif /* __MATRIX_H */
