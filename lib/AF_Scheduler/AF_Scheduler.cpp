#include "AF_Scheduler.h"
#include <system.h>
#include <AF_Logger/AF_Logger.h>

#pragma region AF_Scheduler_Variable_Ids

#define SCHEDULER_AVG_LOOP_TIME_MS  sch.altms

#pragma endregion

/// the default loop frequency, in hz
#define DEFAULT_LOOP_FREQ_HZ        1000U

AF_Scheduler::AF_Scheduler(void) {    
    // set the loop frequency
#if defined(AF_SCHEDULER_LOOP_FREQ_HZ)
    _loop_freq_hz = AF_SCHEDULER_LOOP_FREQ_HZ
#else
     _loop_freq_hz = DEFAULT_LOOP_FREQ_HZ;
#endif
   
}

AF_Scheduler* AF_Scheduler::get_instance(void) {
    if (_instance == nullptr) {
        _instance = new AF_Scheduler();
    }
    return _instance;
}

uint16_t AF_Scheduler::get_loop_time_us(void) const {
    return 1000000U / _loop_freq_hz;
}

void AF_Scheduler::tick(void) {

    // mark the task runner start time
    uint32_t start = AF_HAL::micros();
    // run tasks
    _run_tasks(get_loop_time_us() + _extra_time);
    // mark the task runner end time
    uint32_t end = AF_HAL::micros();
    // increment tick count
    _tick_count++;
    // tell the scheduler how long the loop took to update the running avg
    _notify_loop_runtime(start - end);
}

void AF_Scheduler::register_task(void (*func)(void), uint16_t expected_us, uint16_t freq) {
    // create the task node
    AF_Scheduler_Task_Node* node = new AF_Scheduler_Task_Node();
    // create the task
    node->task = new AF_Scheduler_Task(func, expected_us, freq);
    // place the task into the list
    if (_head == nullptr) {
        _head = node;
        _tail = node;
    } else {
        _tail->next = node;
        _tail = node;
    }

    // keep track of the min task time to prevent unused loop time
    if (expected_us < _min_expected_runtime_us) _min_expected_runtime_us = expected_us;
}