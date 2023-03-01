#ifndef AF_MATH_H_
#define AF_MATH_H_

/// @file   AF_Math.h
/// @brief  contains constants, common math functions, and standardizes types for AutoFlight.

#include <stdlib.h>

/// @brief clamps a value between a minimum and maximum, inclusive.
/// @param value 
/// @param min 
/// @param max 
/// @return the clamped value
double clamp(double value, double min, double max) {
    if (value < min) {
        return min;
    } else if (value > max) {
        return max;
    } else {
        return value;
    }
}

/// @brief  maps a value from one range (in) to another (out), inclusive.
/// @param  value   the value to map
/// @param  in_min  the minimum of the input range
/// @param  in_max  the maximum of the input range
/// @param  out_min the minimum of the output range
/// @param  out_max the maximum of the output range
/// @return the mapped value
double map(double value, double in_min, double in_max, double out_min, double out_max) {
    return (value - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

#endif