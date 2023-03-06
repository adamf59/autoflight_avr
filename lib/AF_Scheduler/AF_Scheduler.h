#ifndef AF_SCHEDULER_H_
#define AF_SCHEDULER_H_

/// @file   AF_Scheduler.h
/// @brief  A basic task scheduler for managing the execution of many tasks
///         at different frequencies and priorities on the cpu.
/// @version 0.1

#include <AF_Variable/AF_Variable.h>
#include <AF_HAL/AF_HAL.h>
#include <system.h>

/// @attention not currently being used, but this idea will be implemented soon.
enum AF_Scheduler_Task_Priority {
    AF_SCHEDULER_TASK_PRIORITY_LO = 0,
    AF_SCHEDULER_TASK_PRIORITY_MD,
    AF_SCHEDULER_TASK_PRIORITY_HI,
};
class AF_Scheduler_Task {

    protected:
            
        /// the function to call for the task
        void (*func)(void);

        /// the expected runtime of the task in microseconds
        uint16_t expected_us;

        /// the frequency of the task in Hz, or 0 for a one-time task
        uint16_t freq;

        /// the priority of the task
        // AF_Scheduler_Task_Priority priority;

        // the next time the task should run
        uint32_t next_run_at = 0;

    public:

        AF_Scheduler_Task(void (*func)(void), uint16_t expected_us, uint16_t freq) {
            this->func = func;
            this->expected_us = expected_us;
            this->freq = freq;
        }

        /// @brief runs the task's function
        void run(uint32_t now) {
            func();
            next_run_at = now + get_period_us();
        }

        /// @brief gets the expected runtime of the task in microseconds
        uint16_t get_expected_us(void) const {
            return expected_us;
        }

        /// @brief gets the period of the task, in microseconds
        inline uint16_t get_period_us(void) const {
            return 1000000U / freq;
        }

        /// @brief gets the frequency of the task in Hz, or 0 for a one-time task
        uint16_t get_freq(void) const {
            return freq;
        }

        /// get the next time the task should run, in system microseconds
        uint32_t get_next_run_at_us() const { return next_run_at; }

        inline bool is_recurring(void) const { return freq > 0; }


};

/// linked-list style node for storing tasks sequentially
struct AF_Scheduler_Task_Node {
    AF_Scheduler_Task_Node* next = nullptr;
    AF_Scheduler_Task* task;
}; 

class AF_Scheduler {

    private:
            /// the single instance of AF_Scheduler 
            static AF_Scheduler* _instance;
            /// how many times per second the scheduler should run
            uint16_t _loop_freq_hz;
            /// the amount of extra time that is usable by the scheduler because tasks didn't run. 
            uint16_t _extra_time = 0;
            /// total ticks that have elapsed.
            /// rolls over to 0 safely.
            uint32_t _tick_count = 0;

            /// head of the linked list containing tasks
            AF_Scheduler_Task_Node* _head = nullptr;
            /// the tail of the linked list containing tasks
            AF_Scheduler_Task_Node* _tail = nullptr;
            /// the node we're currently reading in the linked list
            AF_Scheduler_Task_Node* _read_idx = nullptr;

            // min task expected runtime in microseconds
            uint16_t _min_expected_runtime_us = 0xFFFF;

            /// Private constructor
            AF_Scheduler(void);
            
            /// @brief variable published to the GCS containing the average loop time of the scheduler
            AF_UInt16 _average_loop_time_ms = AF_UInt16("sch.altms", 0, AF_VAR_FLAG_READABLE_BY_GCS | AF_VAR_FLAG_BLACKBOX_LOGGED);

            /// @brief runs registered tasks for the allotted time
            /// @param time_available_us how much time is available for running tasks in microseconds
            void _run_tasks(uint16_t time_available_us);

            /// @brief updates the average runtime of the loop for optimising the scheduler
            /// @param new_runtime_ms the latest runtime of the loop in microseconds
            void _notify_loop_runtime(uint32_t new_runtime_us);

    public:

        /// @brief gets the instance of the scheduler
        AF_Scheduler* get_instance(void);

        /// @brief causes the scheduler to run tasks and collect data (+1 tick)
        void tick(void);

        /// @brief gets the amount of time that will be spent in the loop
        /// @return the loop time, in microseconds
        uint16_t get_loop_time_us(void) const;

        /// @brief registers a new task with the scheduler
        /// @param func the function to call for the task
        /// @param expected_us the expected runtime of the task in microseconds
        /// @param freq the frequency of the task in Hz, or 0 for a one-time task
        void register_task(void (*func)(void), uint16_t expected_us, uint16_t freq);
};

void AF_Scheduler::_run_tasks(uint16_t time_available_us) {

    // keep track of when we started, so we know when to stop
    uint32_t start = AF_HAL::micros();
    int16_t time_left = time_available_us - (AF_HAL::micros() - start);

    // loop through tasks

    while (time_left >= 0) {
        
        AF_Scheduler_Task* cur_task = _read_idx->task;
        uint32_t now = AF_HAL::micros();
        // look at the next task in the list, see if we can run it
        // - due to run AND we have enough time.
        if (cur_task->get_next_run_at_us() <= now && cur_task->get_period_us() <= time_left) {
            // run it
            cur_task->run(now);
            // is the task one time? if so, remove it.
            if (!cur_task->is_recurring()) {
                // TODO: this requires us to make the tasks linked list a doublely linked list
                // so i'll skip the impl for now...
            }
        }        
        // update the read index
        _read_idx = _read_idx->next;
        if (_read_idx == nullptr) _read_idx = _head;
        // finally, update the time left
        time_left = time_available_us - (AF_HAL::micros() - start);
        
        // prevent wasting time (i.e. if we don't have any tasks that can fill remaining time,
        // then we'll be idling in this loop until time_left == 0). this is inefficient.
        // so quit early if we can't do anything.
        if (time_left < _min_expected_runtime_us) {
            _extra_time = time_left;
            return;
        }

    }

    _extra_time = 0;

}

void AF_Scheduler::_notify_loop_runtime(uint32_t new_runtime_us) {
    _average_loop_time_ms = _average_loop_time_ms + ( new_runtime_us / _tick_count );
}

// macro for creating a recurring task
// @param _func the function to call for the task
// @param _expected_us the expected runtime of the task in microseconds
// @param _freq the frequency of the task in Hz
// @return the id of the task
#define AF_SCHEDULER_RECURRING_TASK(_func, _expected_ms, _freq) AF_Scheduler::get_instance()->register_task(_func, _expected_ms, _freq)

// macro for creating a one-time task
// @param _func the function to call for the task
// @param _expected_us the expected runtime of the task in microseconds
// @return the id of the task
#define AF_SCHEDULER_ONE_TIME_TASK(_func, _expected_ms) AF_Scheduler::get_instance()->register_task(_func, _expected_ms, 0)


namespace AF_Logger {

    /// @brief dump task information to the log
    void log_scheduler_task_info(void) {
        // impl
    }

}

#endif

