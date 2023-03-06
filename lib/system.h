#ifndef AF_SYSTEM_H_
#define AF_SYSTEM_H_


/// @file system.h
/// @brief provides support for initializing autoflight without providing explicit system configurations

#include <stdlib.h>
#include <AF_GCS/AF_GCS.h>
#include <AF_HAL/AF_HAL.h>
#include <AF_Scheduler/AF_Scheduler.h>

namespace af_system {

    /// load configurations needed for intialization of autoflight
    void autoflight_load_config();

    /// autoflight entry point, begins AF initialization and scheduler control
    void start() {
        
        // initialize HAL to allow for hardware access
        // AF_HAL::init();

        // initialize core components

        // load configurations
        // af_telemetry::AF_Init_Result init_result = autoflight_load_config();
        // if (!init_result.ok) {
            
        // }

        // give control to the scheduler
        AF_Scheduler::get_instance()->tick_continually();

    }

};

/// macro for exiting autoflight in a critical state
/// should only be called when the system is in an unrecoverable state
/// @warning will immediately cause a complete system reboot, including
///          flight control systems and telemetry.
#define AF_SYSTEM_PANIC() AF_HAL::reset();

#endif