#ifndef __ADDRESSES_H__
#define __ADDRESSES_H__

#define LEFTWHEEL_ADDRESS           0x040 //  8 bytes
#define RIGHTWHEEL_ADDRESS          0x060 //  8 bytes
#define LEFTCODEWHEEL_ADDRESS       0x080 //  8 bytes
#define RIGHTCODEWHEEL_ADDRESS      0x0A0 //  8 bytes
#define ODOMETRY_ADDRESS            0x0C0 //  4 bytes
#define VELOCITYCONTROL_ADDRESS     0x100 // 20 bytes
#define LINVELPID_ADDRESS		    0x140 // 20 bytes
#define ANGVELPID_ADDRESS           0x180 // 20 bytes
#define POSITIONCONTROL_ADDRESS     0x200 // 16 bytes
#define PUREPURSUIT_ADDRESS         0x240 //  4 bytes
#define SMOOTHTRAJECTORY_ADDRESS    0x280 //  4 bytes

#endif // __ADDRESSES_H__
