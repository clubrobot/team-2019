#ifndef __CONSTANTS_H
#define __CONSTANTS_H

#include <Arduino.h>
#include "datatype.h"

/* AX12 SPEED configuration */
constexpr long AX12_SPEED  = 1000000; /* LINK 1 */

/* flip elbow value */
constexpr float FLIP_ELBOW_FRONT = 1;
constexpr float FLIP_ELBOW_BACK  = -1;

/* non reached pos timeout */
constexpr int TIMEOUT      = 10;

/* tasks core */
constexpr float DELTA_T = 0.025;

/* tasks core */
constexpr int RUNNING_CORE = 0;
/* epsilon */
constexpr float EPSILON = 0.001;

#endif /* __CONSTANTS_H */
