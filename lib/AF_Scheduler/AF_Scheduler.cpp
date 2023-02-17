#include "AF_Scheduler.h"
#include <system.h>
#include <AF_Logger/AF_Logger.h>

AF_Scheduler* AF_Scheduler::_instance;


AF_Scheduler::AF_Scheduler(void) {
    // verify that this is the only instance of the scheduler
    if (_instance != nullptr) {
        // AF_LOG_TO_ALL("[AF_Scheduler] multiple schedulers detected.");
        // AF_SYSTEM_PANIC();
    }
}

void AF_Scheduler::start() {

    // start the scheduler
    scheduler_status = true;

    // start the main loop
    while (scheduler_status) {
        _loop();
    }
}

void AF_Scheduler::stop() {
    
    // stop the scheduler
    scheduler_status = false;
}