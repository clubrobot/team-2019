#ifndef __ARM_CONFIG_H
#define __ARM_CONFIG_H

#include <Arduino.h>

/* arm link size configuration */
constexpr double LINK1_LEN = 10.0;
constexpr double LINK2_LEN = 10.0;
constexpr double LINK3_LEN = 3.8;

/* AX12 ID configuration */
constexpr int ID1 = 1; 
constexpr int ID2 = 3; 
constexpr int ID3 = 1; 

/* ARM ORIGIN                     |  x  |  y  | phi | */
constexpr coords_t ORIGIN       = { 0.0 , 0.0 , 0.0 };

/* INITIAL ARM POS                |  x  |  y  | phi | */
constexpr coords_t INITIAL_POS  = { 10.0, 5.0 , 0.0 };

/* Front worksapce coordinate    | Xmin | Xmax | Ymin | Ymax | elbow             */
constexpr workspace_t WS_FRONT = {0     , 20.5 , -5   , 20.5 , FLIP_ELBOW_BACK};

/* Back worksapce coordinate     | Xmin | Xmax | Ymin | Ymax | elbow             */
constexpr workspace_t WS_BACK  = {-20.5 , 0    , -5   , 20.5 , FLIP_ELBOW_FRONT};

/* Delta t for path generation choose the best interval */
constexpr double DELTA_T = 0.5; 

/* null velocity parameter (DO NOT CHANGE THIS VALUE) s*/
constexpr coords_t NULL_VEL = {0.0, 0.0, 0.0};

#endif /* __ARM_CONFIG_H */
