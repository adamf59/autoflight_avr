#include <system.h>
#include <AF_HAL/serial_hal.h>

// System file for running AutoFlight in simulator mode (no hardware)

// enable simulator mode
#define AF_SIMULATOR

void main() {
    // initialize the system
    af_system::start();
}