#ifndef AF_HAL_H_
#define AF_HAL_H_

/// @file   AF_HAL.h
/// @brief  provides an interface for hardware abstraction layer functions
///         such as accessing data buses, analog and digital pins, and timers.

#include "system_hal.h"
#include <stdlib.h>
#include <stdint.h>

namespace AF_HAL {

    /// @brief  initializes the hardware abstraction layer
    void init();

    namespace io {
        /// @brief  reads a value from an analog pin
        /// @param  pin     the pin to read from
        /// @return the value read from the pin
        uint16_t aread(uint8_t pin);

        /// @brief          writes a value to an analog pin
        /// @param pin      the pin to write to
        /// @param value    the value to write to the pin, clamped to 0-1023
        void awrite(uint8_t pin, uint16_t value);

        /// @brief          reads a value from a digital pin
        /// @param pin      the pin to read from
        /// @return         the value read from the pin
        bool dread(uint8_t pin);

        /// @brief          writes a value to a digital pin
        /// @param  pin     the pin to write to
        /// @param  value   the value to write to the pin
        void dwrite(uint8_t pin, int value);

        /// @brief          sets a pin to be an input, output, or input with pullup
        /// @param pin      the pin to set
        /// @param mode     the mode to set the pin to
        void pmode(uint8_t pin, int mode);

        /// @brief generates a pwm signal on a pin
        /// @param pin 
        /// @param value    the value to write to the pin, clamped to 0-1023
        void pwmwrite(uint8_t pin, uint16_t value);
    }

}

#endif