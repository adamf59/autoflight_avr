#ifndef AF_HAL_H_
#define AF_HAL_H_

/// @file   AF_HAL.h
/// @brief  provides an interface for hardware abstraction layer functions
///         such as accessing data buses, analog and digital pins, and timers.

#include "system_hal.h"

namespace AF_HAL {

    /// @brief  initializes the hardware abstraction layer
    void init();

    /// @brief  reads a value from an analog pin
    /// @param  pin     the pin to read from
    /// @return the value read from the pin
    int aread(int pin);

    /// @brief  writes a value to an analog pin
    /// @param  pin     the pin to write to
    /// @param  value   the value to write to the pin
    void awrite(int pin, int value);

    /// @brief  reads a value from a digital pin
    /// @param  pin     the pin to read from
    /// @return the value read from the pin
    int dread(int pin);

    /// @brief  writes a value to a digital pin
    /// @param  pin     the pin to write to
    /// @param  value   the value to write to the pin
    void dwrite(int pin, int value);

    


}