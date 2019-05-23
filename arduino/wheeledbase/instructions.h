#ifndef __INSTRUCTIONS_H__
#define __INSTRUCTIONS_H__

#include "../common/SerialTalks.h"

// Opcodes declaration

#define SET_OPENLOOP_VELOCITIES_OPCODE  0x13 //0x12

#define GET_CODEWHEELS_COUNTERS_OPCODE  0x1C //0x0D

#define SET_VELOCITIES_OPCODE           0x10 //0x06

#define START_PUREPURSUIT_OPCODE        0x11 //0x07
#define START_TURNONTHESPOT_OPCODE      0x12 //0x09

#define POSITION_REACHED_OPCODE         0x14 //0x08

#define SET_POSITION_OPCODE             0x15 //0x0A
#define GET_POSITION_OPCODE             0x16 //0x0B
#define GET_VELOCITIES_OPCODE           0x17 //0x0C

#define SET_PARAMETER_VALUE_OPCODE      0x18 //0x0E
#define GET_PARAMETER_VALUE_OPCODE      0x19 //0x0F

#define RESET_PUREPURSUIT_OPCODE        0x1A //0x10
#define ADD_PUREPURSUIT_WAYPOINT_OPCODE 0x1B //0x11

#define GET_VELOCITIES_WANTED_OPCODE    0x1D
#define GOTO_DELTA_OPCODE               0x1E

#define RESET_PARAMETERS_OPCODE         0x1F
#define SAVE_PARAMETERS_OPCODE          0x20
#define START_TURNONTHESPOT_DIR_OPCODE  0x21
// Parameters identifiers

#define LEFTWHEEL_RADIUS_ID             0x10
#define LEFTWHEEL_CONSTANT_ID           0x11
#define LEFTWHEEL_MAXPWM_ID             0x12
#define RIGHTWHEEL_RADIUS_ID            0x20
#define RIGHTWHEEL_CONSTANT_ID          0x21
#define RIGHTWHEEL_MAXPWM_ID            0x22
#define LEFTCODEWHEEL_RADIUS_ID         0x40
#define LEFTCODEWHEEL_COUNTSPERREV_ID   0x41
#define RIGHTCODEWHEEL_RADIUS_ID        0x50
#define RIGHTCODEWHEEL_COUNTSPERREV_ID  0x51
#define ODOMETRY_AXLETRACK_ID           0x60
#define ODOMETRY_SLIPPAGE_ID            0x61
#define VELOCITYCONTROL_AXLETRACK_ID    0x80
#define VELOCITYCONTROL_MAXLINACC_ID    0x81
#define VELOCITYCONTROL_MAXLINDEC_ID    0x82
#define VELOCITYCONTROL_MAXANGACC_ID    0x83
#define VELOCITYCONTROL_MAXANGDEC_ID    0x84
#define VELOCITYCONTROL_SPINSHUTDOWN_ID 0x85
#define LINVELPID_KP_ID                 0xA0
#define LINVELPID_KI_ID                 0xA1
#define LINVELPID_KD_ID                 0xA2
#define LINVELPID_MINOUTPUT_ID          0xA3
#define LINVELPID_MAXOUTPUT_ID          0xA4
#define ANGVELPID_KP_ID                 0xB0
#define ANGVELPID_KI_ID                 0xB1
#define ANGVELPID_KD_ID                 0xB2
#define ANGVELPID_MINOUTPUT_ID          0xB3
#define ANGVELPID_MAXOUTPUT_ID          0xB4
#define POSITIONCONTROL_LINVELKP_ID     0xD0
#define POSITIONCONTROL_ANGVELKP_ID     0xD1
#define POSITIONCONTROL_LINVELMAX_ID    0xD2
#define POSITIONCONTROL_ANGVELMAX_ID    0xD3
#define POSITIONCONTROL_LINPOSTHRESHOLD_ID  0xD4
#define POSITIONCONTROL_ANGPOSTHRESHOLD_ID  0xD5
#define PUREPURSUIT_LOOKAHED_ID         0xE0
#define PUREPURSUIT_LOOKAHEADBIS_ID     0xE2

#define BORNIBUS    0
#define R128        1


// Instructions prototypes
void DISABLE(SerialTalks& talks, Deserializer& input, Serializer& output);

void GOTO_DELTA(SerialTalks& talks, Deserializer& input, Serializer& output);

void RESET_PARAMETERS(SerialTalks& talks, Deserializer& input, Serializer& output);

void SET_OPENLOOP_VELOCITIES(SerialTalks& talks, Deserializer& input, Serializer& output);

void GET_CODEWHEELS_COUNTERS(SerialTalks& talks, Deserializer& input, Serializer& output);

void SET_VELOCITIES(SerialTalks& talks, Deserializer& input, Serializer& output);

void RESET_PUREPURSUIT(SerialTalks& talks, Deserializer& input, Serializer& output);

void ADD_PUREPURSUIT_WAYPOINT(SerialTalks& talks, Deserializer& input, Serializer& output);

void START_PUREPURSUIT(SerialTalks& talks, Deserializer& input, Serializer& output);

void START_TURNONTHESPOT(SerialTalks& talks, Deserializer& input, Serializer& output);

void START_TURNONTHESPOT_DIR(SerialTalks& talks, Deserializer& input, Serializer& output);

void POSITION_REACHED(SerialTalks& talks, Deserializer& input, Serializer& output);

void GET_VELOCITIES_WANTED(SerialTalks& talks, Deserializer& input, Serializer& output);

void SET_POSITION(SerialTalks& talks, Deserializer& input, Serializer& output);

void GET_POSITION(SerialTalks& talks, Deserializer& input, Serializer& output);

void GET_VELOCITIES(SerialTalks& talks, Deserializer& input, Serializer& output);

void SET_PARAMETER_VALUE(SerialTalks& talks, Deserializer& input, Serializer& output);

void GET_PARAMETER_VALUE(SerialTalks& talks, Deserializer& input, Serializer& output);

void RESET_PARAMETERS(SerialTalks& talks, Deserializer& input, Serializer& output);

void SAVE_PARAMETERS(SerialTalks& talks, Deserializer& input, Serializer& output);


#endif // __INSTRUCTIONS_H__
