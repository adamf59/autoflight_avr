#ifndef AF_SYSTEM_H_
#define AF_SYSTEM_H_


/// @file system.h
/// @brief provides support for initializing autoflight without providing explicit system configurations

#include <stdlib.h>
#include <AF_GCS/AF_GCS.h>
#include <AF_HAL/AF_HAL.h>

namespace af_telemetry {

    typedef struct {
        bool ok;
        char msg;
    } AF_Init_Result;

    /// convienence macro for building an AF_Init_Result
    #define AF_INIT_RESULT(_ok, _msg) { .ok = _ok, .msg = _msg }

}

namespace af_system {

    /// load configurations needed for intialization of autoflight
    void autoflight_load_config();

    /// begins initialization of autoflight 
    af_telemetry::AF_Init_Result autoflight_init() {

        // initialize sensors

        // initialize flight surfaces and thrusters

        // initialize GCS / telemetry
        AF_GCS * gcs = af_gcs::init();
        
    }

    /// autoflight entry point, begins AF initialization and scheduler control
    void start() {
        
        // initialize HAL to allow for hardware access
        AF_HAL::init();

        // initialize core components

        // load configurations
        // af_telemetry::AF_Init_Result init_result = autoflight_load_config();
        // if (!init_result.ok) {
            
        // }

        // give control to the scheduler

    }

};

/// macro for exiting autoflight in a critical state
/// should only be called when the system is in an unrecoverable state
/// @warning will immediately cause a complete system reboot, including
///          flight control systems and telemetry.
#define AF_SYSTEM_PANIC() AF_HAL::reset();

#define F_CPU 16000000UL

#endif