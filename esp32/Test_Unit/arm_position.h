#ifndef __ARM_POSITION
#define __ARM_POSITION

#include <Arduino.h>
#include "datatype.h"

typedef enum
{
    HOME                = 0,
    PUCK_POS            = 1,
    PUCK_POS_INTER      = 2,
    TANK_POS_INTER      = 3,
    MID_POS             = 4,
    TANK_POS_0          = 5,
    TANK_POS_1          = 6,
    TANK_POS_2          = 7,
    BALANCE_POS_INTER   = 8,
    BALANCE_POS         = 9,
    TANK_POS_INTER_2    = 10,
    PUCK_POS_INTER_1    = 11,
}position_t;

static coords_t arm_positions[] = 
{
    [HOME]              = {10.0, 10.0, 0},
    [PUCK_POS]          = {-14.0, 8.2, M_PI},
    [PUCK_POS_INTER]    = {-10.0, 8.2, M_PI},
    [TANK_POS_INTER]    = {10.0, 10.0, 0},
    [MID_POS]           = {0, 23.8, M_PI/2},
    [TANK_POS_0]        = {7.0, 0.0, -M_PI/2},
    [TANK_POS_1]        = {7.0, 2.0, -M_PI/2},
    [TANK_POS_2]        = {7.0, 4.0, -M_PI/2},
    [BALANCE_POS_INTER] = {7.0, 5.0, 0},
    [BALANCE_POS]       = {7.0, 5.0, 0},
    [TANK_POS_INTER_2]  = {7.0, 6.0, -M_PI/2},
    [PUCK_POS_INTER_1]  = {-13.0, 8.2, M_PI},
};

#endif