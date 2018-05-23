#ifndef __CONSTANTS_H__
#define __CONSTANTS_H__

// DC motors constants

#define DCMOTORS_VELOCITY_CONSTANT 622 // RPM
#define DCMOTORS_REDUCTION_RATIO    19
#define DCMOTORS_SUPPLIED_VOLTAGE   11 // V

// Codewheels constants

#define CODEWHEELS_COUNTS_PER_REVOLUTION 10000

// Mechanical constants

#define LEFT_WHEEL_RADIUS   23.8 // mm
#define RIGHT_WHEEL_RADIUS  23.8 // mm
#define WHEELS_AXLE_TRACK  125.0 // mm

#define LEFT_CODEWHEEL_RADIUS   22.71 // mm
#define RIGHT_CODEWHEEL_RADIUS  22.71 // mm
#define CODEWHEELS_AXLE_TRACK  209.16 // mm

// Really traveled distance > thought traveled distance -> decrease codewheels radius
// Really rotated angle > thought rotated angle         -> increase axle track
// Turn right when thinking moving forward              -> increase left codewheel radius
// Turn left when thinking moving forward               -> increase right codewheel radius

// Trajectory constants

#define MIN_LINEAR_POSITION        1 // mm
#define MIN_ANGULAR_POSITION     0.1 // rad

#define MAX_LINEAR_VELOCITY      500 // mm/s
#define MAX_ANGULAR_VELOCITY     1.5 // rad/s^2

#define MAX_LINEAR_ACCELERATION    500 // mm/s^2
#define MAX_LINEAR_DECCELERATION  1000 // mm/s^2
#define MAX_ANGULAR_ACCELERATION  3.14 // rad/s^2
#define MAX_ANGULAR_DECCELERATION 6.28 // rad/s^2

// Timesteps

#define ODOMETRY_TIMESTEP          5e-3 // s
#define PID_CONTROLLERS_TIMESTEP  20e-3 // s
#define POSITIONCONTROL_TIMESTEP  50e-3 // s

#endif // __CONSTANTS_H__
