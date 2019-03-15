#ifndef __ADDRESSES_H__
#define __ADDRESSES_H__

#define LEFTWHEEL_ADDRESS           0x040 //  12 bytes + 2 safe = 14 | 0X040 -> 0X04E
#define RIGHTWHEEL_ADDRESS          0x04F //  12 bytes + 2 safe = 14 | 0X04F -> 0X05D
#define LEFTCODEWHEEL_ADDRESS       0x05E //  8 bytes + 2 safe = 10  | 0X05E -> 0X068
#define RIGHTCODEWHEEL_ADDRESS      0x069 //  8 bytes + 2 safe = 10  | 0X069 -> 0X073
#define ODOMETRY_ADDRESS            0x074 //  8 bytes + 2 safe = 10  | 0X074 -> 0X07E
#define VELOCITYCONTROL_ADDRESS     0x07F // 21 bytes + 2 safe = 23  | 0X07F -> 0X096
#define LINVELPID_ADDRESS		    0x097 // 20 bytes + 2 safe = 22  | 0X097 -> 0X0AD
#define ANGVELPID_ADDRESS           0x0AE // 20 bytes + 2 safe = 22  | 0X0AE -> 0X0C4
#define POSITIONCONTROL_ADDRESS     0x0C5 // 24 bytes + 2 safe = 26  | 0X0C5 -> 0X0DF
#define PUREPURSUIT_ADDRESS         0x0E9 //  8 bytes + 2 safe = 10  | 0X0E9 -> 0X0EA
#define SMOOTHTRAJECTORY_ADDRESS    0x0EB //  8 bytes + 2 safe = 10  | 0X0EB -> 0X0F5

#endif // __ADDRESSES_H__
