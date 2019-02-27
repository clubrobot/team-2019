#ifndef __ARM_CONFIG_H
#define __ARM_CONFIG_H

#include <Arduino.h>
#include "datatype.h"

/* THIS FILE IS USED FOR ARM CONFIGURATION, PLEASE ONLY CONFIGURE VALUE ON THIS FILE */

/* arm link size configuration */
constexpr double LINK1_LEN_DEFAULT = 10.0;
constexpr double LINK2_LEN_DEFAULT = 10.0;
constexpr double LINK3_LEN_DEFAULT = 5.0;

/* links offsets config */
constexpr double LINK1_OFFSET_DEFAULT = 60.0;
constexpr double LINK2_OFFSET_DEFAULT = 150.0;
constexpr double LINK3_OFFSET_DEFAULT = 150.0;

/* AX12 ID configuration */
constexpr int ID1_DEFAULT = 1; /* LINK 1 */
constexpr int ID2_DEFAULT = 3; /* LINK 2 */
constexpr int ID3_DEFAULT = 2; /* LINK 3 */

/* flip elbow value */
constexpr double FLIP_ELBOW_FRONT = 1;
constexpr double FLIP_ELBOW_BACK  = -1;

/* ARM ORIGIN                     |  x  |  y  | phi | */
constexpr coords_t ORIGIN_DEFAULT       = { 0.0 , 0.0 , 0.0 };

/* INITIAL ARM POS                |  x  |  y  | phi | */
constexpr coords_t INITIAL_POS_DEFAULT  = { 10 , 10, 0.0 };

/* INITIAL ARM JOINT                        |  th1  |  th2  | th3 | */
constexpr joints_t INITIAL_JOINTS_DEFAULT  = { 150 , 150, 150 };

/* Front worksapce coordinate    | Xmin | Xmax | Ymin | Ymax | elbow             */
constexpr workspace_t WS_FRONT_DEFAULT = {0     , 30 , -10  , 30 , FLIP_ELBOW_BACK};

/* Back worksapce coordinate     | Xmin | Xmax | Ymin | Ymax | elbow             */
constexpr workspace_t WS_BACK_DEFAULT  = {-30 , 0    , -10   , 30 , FLIP_ELBOW_FRONT};

/* Delta t for path generation choose the best interval */
constexpr double DELTA_T = 0.025; 

/* non reached pos timeout */
constexpr int TIMEOUT      = 10;

/************** NOT CHANGE VALUES BELOW **********************/
/* null velocity parameter*/
constexpr coords_t NULL_VEL = {0.0, 0.0, 0.0};

/* tasks core */
constexpr int RUNNING_CORE = 0;
/* epsilon */
constexpr double EPSILON = 0.001;

#endif /* __ARM_CONFIG_H */
