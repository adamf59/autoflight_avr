#include <system.h>
#include <AF_HAL/serial_hal.h>

// System file for running AutoFlight on an ATmega2560 

// enable serial communication
#define AF_SERIAL_AVAILABLE

// define the chip clock speed
#define F_CPU 16000000UL

// entry point for the program
void main() {
    // initialize the system
    af_system::start();

    AF_HAL::hwserial::SerialInterface0.open(BAUDR_57600);
    // AF_HAL::hwserial::SerialInterface0.

}