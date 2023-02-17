#ifndef AF_HAL_SYSTEM_HAL_H_
#define AF_HAL_SYSTEM_HAL_H_

/// @file   system_hal.h
/// @brief  provides an interface for system-level functions such as
///         rebooting the system, and reading the system clock.

#include <stdint.h>

namespace AF_HAL {

    /// @brief  initializes the avr system
    void init();

    /// @brief  resets the avr system
    void reset();

    /// @brief  reads the system clock
    /// @return the system clock in microseconds
    uint32_t micros();

}

#endif // AF_HAL_SYSTEM_HAL_H_