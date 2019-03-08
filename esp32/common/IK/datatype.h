#ifndef __DATATYPE_H
#define __DATATYPE_H

#include <Arduino.h>
#include <vector>

using namespace std;

struct Coords
{
	Coords() : x(0), y(0), phi(0){}
	Coords(float x, float y, float phi) : x(x), y(y), phi(phi){}

	float x, y;
	float phi;
};

struct Joints
{
	Joints() : th1(0), th2(0), th3(0){}
	Joints(float th1, float th2, float th3) : th1(th1), th2(th2), th3(th3){}

	float th1, th2, th3;
};


typedef struct 
{
	vector<float> t;
	vector<float> pos;
	vector<float> vel;
	vector<float> acc;
}vector_t;

struct DetailedPos
{
	DetailedPos(){}
    DetailedPos(Coords origin, Coords link1, Coords link2, Coords tool): \
        origin(origin), link1(link1), link2(link2), tool(tool){}

	Coords origin;
	Coords link1;
	Coords link2;
	Coords tool; 
};

typedef struct
{
	vector_t path_th1;
	vector_t path_th2;
	vector_t path_th3;

	Coords pos;
}path_t;

struct Polynom
{
    Polynom() : a0(0), a1(0), a2(0){}
	Polynom(float a0, float a1, float a2) : a0(a0), a1(a1), a2(a2){}

	float a0;
	float a1;
	float a2;
};

struct Pos
{
    Pos() : min(0), max(0){}
	Pos(float min, float max) : min(min), max(max){}

    float min;
	float max;
};

struct Vel
{
    Vel() : min(0), max(0){}
	Vel(float min, float max) : min(min), max(max){}

	float min;
	float max;
};

struct Acc
{
    Acc() : min(0), max(0){}
	Acc(float min, float max) : min(min), max(max){}

	float min;
	float max;
};

struct Constraints
{
	Constraints(){}
	Constraints(Pos pos, Vel vel, Acc acc) : pos(pos), vel(vel), acc(acc){}

    Pos pos;
    Vel vel;
    Acc acc;
};

struct TrajectoryTime
{
    TrajectoryTime() : t1(0), t2(0), tf(0){}
	TrajectoryTime(float t1, float t2, float tf) : t1(t1), t2(t2), tf(tf){}

	float t1;
	float t2;
	float tf;
};

struct Workspace
{
    Workspace() : x_min(0), x_max(0), y_min(0), y_max(0), elbow_or(1) {}

	Workspace(float x_min, float x_max, float y_min, float y_max, int elbow_or):\
            x_min(x_min), x_max(x_max), y_min(y_min), y_max(y_max),elbow_or(elbow_or) {}

    float x_min;
    float x_max;

    float y_min;
    float y_max;

    float elbow_or;
};

#endif /* __DATATYPE_H */