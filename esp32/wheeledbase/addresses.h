#ifndef __ADDRESSES_H__
#define __ADDRESSES_H__

#define LEFTWHEEL_ADDRESS           0x040 //  8 bytes
#define RIGHTWHEEL_ADDRESS          0x0A6 //  8 bytes
#define LEFTCODEWHEEL_ADDRESS       0x0CE //  8 bytes
#define RIGHTCODEWHEEL_ADDRESS      0x0F6 //  8 bytes
#define ODOMETRY_ADDRESS            0x11E //  4 bytes
#define VELOCITYCONTROL_ADDRESS     0x146 // 20 bytes
#define LINVELPID_ADDRESS		    0x16E // 20 bytes
#define ANGVELPID_ADDRESS           0x196 // 20 bytes
#define POSITIONCONTROL_ADDRESS     0x1BE // 16 bytes
#define PUREPURSUIT_ADDRESS         0x1E6 //  4 bytes
#define SMOOTHTRAJECTORY_ADDRESS    0x1F0 //  4 bytes

#endif // __ADDRESSES_H__
