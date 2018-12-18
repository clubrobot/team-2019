#ifndef __DATATYPE_H
#define __DATATYPE_H

#include <Arduino.h>
#include <vector>

using namespace std;
/* coords struct */
typedef struct
{
	double x;
	double y;
	double phi;
}coords_t;

/* joints angles struct */
typedef struct
{
	double th1;
	double th2;
	double th3;

	bool intermediary_pos;

	double th1_int;
	double th2_int;
	double th3_int;
	
}joints_t;

/* vector type struct */
typedef struct 
{
	vector<double> t;
	vector<double> pos;
	vector<double> vel;
	vector<double> acc;
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
	double a0;
	double a1;
	double a2;
}polynom_t;

/* constraints struct */
typedef struct
{
	double pos_min;
	double pos_max;

	double vel_min;
	double vel_max;

	double acc_min;
	double acc_max;
}constraints_t;

/* trajectory time struct */
typedef struct
{
	double t1;
	double t2;
	double tf;
}trajectory_time_t;

/* workaspaces struct */
typedef struct
{
    double x_min;
    double x_max;

    double y_min;
    double y_max;

    double elbow_orientation;
}workspace_t;


#endif /* __DATATYPE_H */