#ifndef _GEOMETRY_TYPE_H_
#define _GEOMETRY_TYPE_H_
#define MAX_POLYGONE_LENGTH 64
typedef struct {int x; int y;} Point;
typedef struct {int length; Point list[MAX_POLYGONE_LENGTH];} Polygon;
typedef struct {float linear; float angular;} Velocity;
typedef Velocity Accelerator;
#endif //_GEOMETRY_TYPE_H_