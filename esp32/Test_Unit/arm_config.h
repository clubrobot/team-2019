#ifndef __ARM_CONFIG_H
#define __ARM_CONFIG_H

#include <Arduino.h>

constexpr double LINK1_LEN = 10.0;
constexpr double LINK2_LEN = 10.0;
constexpr double LINK3_LEN = 3.8;
//                                  x    y   phi 
constexpr coords_t ORIGIN       = {0.0, 0.0, 0.0};
constexpr coords_t INITIAL_POS  = {10.0, 5.0, 0.0};

constexpr int ID1 = 1; 
constexpr int ID2 = 3; 
constexpr int ID3 = 1; 

#endif /* __ARM_CONFIG_H */
