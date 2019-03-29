#ifndef __ADDRESSES_H__
#define __ADDRESSES_H__

#define MOTOR1_ADDRESS              0x040 //  8 bytes + 2 safe = 10  | 0X040 -> 0X04A
#define MOTOR2_ADDRESS              0x04B //  8 bytes + 2 safe = 10  | 0X04B -> 0X055
#define MOTOR3_ADDRESS              0x056 //  8 bytes + 2 safe = 10  | 0X056 -> 0X060

#define ARM_MANAGER_ADDRESS         0x061 //  80 bytes + 2 safe = 82 | 0X061 -> 0X0B3
#define TRAJ_MANAGER_ADDRESS        0x0B4 //  4 bytes  + 2 safe = 6  | 0X0B4 -> 0X0BA

#endif // __ADDRESSES_H__
