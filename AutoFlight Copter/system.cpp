#include <system.h>

// System file for running AutoFlight on an ATmega2560 

// enable serial communication
#define AF_SERIAL_AVAILABLE

// define the chip clock speed
#define F_CPU 16000000UL

// entry point for the program
void main() {
    // initialize the system
    af_system::start();
}