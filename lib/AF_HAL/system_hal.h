#ifndef AF_HAL_SYSTEM_HAL_H_
#define AF_HAL_SYSTEM_HAL_H_

/// @file   system_hal.h
/// @brief  provides an interface for system-level functions such as
///         rebooting the system, and reading the system clock.

#include "AF_HAL.h"

/// @brief  pin mode for an input
#define IO_MODE_INPUT         0x00
/// @brief  pin mode for an output
#define IO_MODE_OUTPUT        0x01
/// @brief  pin mode for an input with pullup resistor
#define IO_MODE_INPUT_PULLUP  0x02
/// @brief  the value of a digital pin when it is logical 0
#define IO_LOGICAL_0          0x00
/// @brief  the value of a digital pin when it is logical 1
#define IO_LOGICAL_1          0x01

namespace AF_HAL {

    /// @brief  initializes the avr system
    void init();

    /// @brief  resets the avr system
    void reset();

    /// @brief  reads the system clock
    /// @return the system clock in microseconds
    uint32_t micros(void);

}

#endif // AF_HAL_SYSTEM_HAL_H_