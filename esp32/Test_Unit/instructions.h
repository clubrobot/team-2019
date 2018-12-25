#ifndef __INSTRUCTIONS_H__
#define __INSTRUCTIONS_H__

#include "../common/IK/Picker.h"
#include "../common/SerialTalks.h"

#define MOVE_DIRECTLY_OPCODE      0x15
#define IS_ARRIVED_OPCODE         0x16

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
}position_t;

static coords_t arm_positions[] = 
{
    [HOME]              = {10.0, 15.0, 0},
    [PUCK_POS]          = {-12.0, 15.0, M_PI},
    [PUCK_POS_INTER]    = {-8.0, 15.0, M_PI},
    [TANK_POS_INTER]    = {10.0, 10.0, 0},
    [MID_POS]           = {0, 23.8, M_PI/2},
    [TANK_POS_0]        = {10.0, 0.0, -M_PI/2},
    [TANK_POS_1]        = {10.0, 2.0, -M_PI/2},
    [TANK_POS_2]        = {10.0, 4.0, -M_PI/2},
    [BALANCE_POS_INTER] = {10.0, 5.0, 0},
    [BALANCE_POS]       = {10.0, 5.0, 0},
};

void MOVE_DIRECTLY(SerialTalks& talks, Deserializer& input, Serializer& output);

void IS_ARRIVED(SerialTalks& talks, Deserializer& input, Serializer& output);

#endif //__INSTRUCTIONS_H__

