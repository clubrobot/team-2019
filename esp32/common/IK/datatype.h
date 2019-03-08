#ifndef __DATATYPE_H
#define __DATATYPE_H

#include <Arduino.h>
#include <vector>

using namespace std;
/* coords struct */
typedef struct
{
	float x;
	float y;
	float phi;
}coords_t;

/* joints angles struct */
typedef struct
{
	float th1;
	float th2;
	float th3;

}joints_t;

/* vector type struct */
typedef struct 
{
	vector<float> t;
	vector<float> pos;
	vector<float> vel;
	vector<float> acc;
}vector_t;

/* detailled pos struct */
typedef struct
{
	coords_t origin;
	coords_t link1;
	coords_t link2;
	coords_t tool; 
}detailed_pos_t;

/* path struct */
typedef struct
{
	vector_t path_th1;
	vector_t path_th2;
	vector_t path_th3;

	coords_t pos;
}path_t;

/* polynom struct */
typedef struct
{
	float a0;
	float a1;
	float a2;
}polynom_t;

/* constraints struct */
typedef struct
{
	float pos_min;
	float pos_max;

	float vel_min;
	float vel_max;

	float acc_min;
	float acc_max;
}constraints_t;

/* trajectory time struct */
typedef struct
{
	float t1;
	float t2;
	float tf;
}trajectory_time_t;

/* workaspaces struct */
typedef struct
{
    float x_min;
    float x_max;

    float y_min;
    float y_max;

    float elbow_orientation;
}workspace_t;


#endif /* __DATATYPE_H */