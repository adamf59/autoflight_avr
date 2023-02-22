#ifndef AF_SCHEDULER_H_
#define AF_SCHEDULER_H_

/// @file   AF_Scheduler.h
/// @brief  A basic task scheduler for managing the execution of many tasks
///         at different frequencies and priorities on the cpu.
/// @version 0.1

#include <AF_Variable/AF_Variable.h>
#include <AF_HAL/AF_HAL.h>

enum AF_Scheduler_Task_Priority {
    AF_SCHEDULER_TASK_PRIORITY_LO = 0,
    AF_SCHEDULER_TASK_PRIORITY_MD,
    AF_SCHEDULER_TASK_PRIORITY_HI,
};

typedef uint16_t af_sched_task_id_t;

class AF_Scheduler_Task_Spec {

    protected:
            
        /// the function to call for the task
        void (*func)(void);

        /// the expected runtime of the task in milliseconds
        uint16_t expected_ms;

        /// the frequency of the task in Hz, or 0 for a one-time task
        uint16_t freq;

        /// the priority of the task
        AF_Scheduler_Task_Priority priority;

        uint32_t average_runtime_ms = 0;

    public:

    AF_Scheduler_Task_Spec(void (*func)(void), uint16_t expected_ms, uint16_t freq, AF_Scheduler_Task_Priority priority) {
        this->func = func;
        this->expected_ms = expected_ms;
        this->freq = freq;
        this->priority = priority;
    }

    /// @brief gets the function to call for the task
    void (*get_func(void))(void) {
        return func;
    }

    /// @brief gets the expected runtime of the task in milliseconds
    uint16_t get_expected_ms(void) {
        return expected_ms;
    }

    /// @brief gets the frequency of the task in Hz, or 0 for a one-time task
    uint16_t get_freq(void) {
        return freq;
    }

    inline bool is_recurring(void) { return freq > 0; }

    /// @brief gets the priority of the task
    AF_Scheduler_Task_Priority get_priority(void) {
        return priority;
    }

};

class AF_Scheduler_Task_Spec_Recurring: public AF_Scheduler_Task_Spec  {

    private:
            
            uint32_t average_runtime_ms = 0;

    public:

        AF_Scheduler_Task_Spec_Recurring(void (*func)(void), uint16_t expected_ms, uint16_t freq, AF_Scheduler_Task_Priority priority) : AF_Scheduler_Task_Spec(func, expected_ms, freq, priority) {
            
        }

        /// @brief gets the average runtime of this task 
        uint32_t get_average_runtime_ms(void) {
            return average_runtime_ms;
        }

        /// @brief updates the average runtime of the task for optimising the scheduler
        /// @param new_runtime_ms the latest runtime of the task in milliseconds
        void notify_runtime(uint32_t new_runtime_ms) {
            this->average_runtime_ms = average_runtime_ms;
        }

};

class AF_Scheduler {

    private:
            
            /// singleton instance of the scheduler
            static AF_Scheduler * _instance;

            AF_Bool scheduler_status = AF_Bool("sch.status", false);

            /// @brief counter for task_ids  
            af_sched_task_id_t _next_task_id = 0;

            AF_UInt16 _average_loop_time_ms = AF_UInt16("sch.avg_loop_time_ms", 0);

            /// vector containing recurring tasks
            // std::vector<AF_Scheduler_Task_Spec> _recurring_tasks;
            // std::map<af_sched_task_id_t, AF_Scheduler_Task_Spec> task_map;

            /// @brief the main loop of the scheduler 
            void _loop(void);

    public:

        /// @brief create a new scheduler
        AF_Scheduler(void);

        /// @brief get the singleton instance of the scheduler
        static AF_Scheduler * get_singleton(void) { return _instance; }

        /// @brief registers a new task with the scheduler
        /// @param func the function to call for the task
        /// @param expected_ms the expected runtime of the task in milliseconds
        /// @param freq the frequency of the task in Hz, or 0 for a one-time task
        /// @param priority the priority of the task
        /// @return the id of the task
        af_sched_task_id_t register_task(void (*func)(void), uint16_t expected_ms, uint16_t freq, AF_Scheduler_Task_Priority priority);

        /// @brief updates the average runtime of the loop for optimising the scheduler
        /// @param new_runtime_ms the latest runtime of the loop in milliseconds
    
        void notify_loop_runtime(uint32_t new_runtime_ms);

        /// @brief starts the scheduler
        void start();

        /// @brief stops the scheduler
        void stop();
};

void AF_Scheduler::_loop() {

    // end time for the loop
    uint32_t l_start = AF_HAL::micros();
    
    // run the tasks
    
    // calculate the time spent in the loop, recompute runtime average for the loop
    uint32_t l_runtime = AF_HAL::micros() - l_start;
    notify_loop_runtime(l_runtime);
    
}

// macro for creating a recurring task
// @param _func the function to call for the task
// @param _expected_ms the expected runtime of the task in milliseconds
// @param _freq the frequency of the task in Hz
// @param _priority the priority of the task
// @return the id of the task
#define AF_SCHEDULER_RECURRING_TASK(_func, _expected_ms, _freq, _priority) AF_Scheduler::get_singleton()->register_task(_func, _expected_ms, _freq, _priority)

// macro for creating a one-time task
// @param _func the function to call for the task
// @param _expected_ms the expected runtime of the task in milliseconds
// @param _priority the priority of the task
// @return the id of the task
#define AF_SCHEDULER_ONE_TIME_TASK(_func, _expected_ms, _priority) AF_Scheduler::get_singleton()->register_task(_func, _expected_ms, 0, _priority)


namespace AF_Logger {

    /// @brief dump task information to the log
    void log_scheduler_task_info(void) {
        // impl
    }

}

#endif

