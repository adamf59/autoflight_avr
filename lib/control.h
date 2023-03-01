#ifndef CONTROL_H_
#define CONTROL_H_

    /// @file control.h
    /// @brief classses for different control methods

#include <stdint.h>
#include <AF_Variable/AF_Variable.h>

class PID {

    private:

        /// the proportional term
        AF_Float _kp;
        /// the integral term
        AF_Float _ki;
        /// the derivative term
        AF_Float _kd;
        /// the bias
        AF_Float _bias;
        /// the maximum output, which the output will be clamped to
        float _max_output;
        /// the minimum output, which the output will be clamped to
        float _min_output;
        /// the maximum integral, which the integral will be clamped to
        float _max_integral;
        /// the minimum integral, which the integral will be clamped to
        float _min_integral;

        /// the last error
        float _last_error;
        /// the integral of the error
        float _integral;

    public:

        /// @brief  constructor
        /// @param  kp  the proportional term
        /// @param  ki  the integral term
        /// @param  kd  the derivative term
        /// @param  bias    the bias
        /// @param  max_output  the maximum output, which the output will be clamped to
        /// @param  min_output  the minimum output, which the output will be clamped to
        /// @param  max_integral    the maximum integral, which the integral will be clamped to
        /// @param  min_integral    the minimum integral, which the integral will be clamped to
        /// @param  var_flags   the flags for the AF_Floats
        /// @param  prefix  the prefix identifiers for the AF_Floats
        PID(float kp, float ki, float kd, float bias, float max_output, float min_output, float max_integral, float min_integral, uint8_t var_flags, const char* prefix);

        /// @brief computes the output of the PID controller
        /// @param error   the error
        /// @return the output of the PID controller
        float output(float error);

        /// @brief peeks at the output of the PID controller, without updating the integral or derivative terms
        /// @param error the error
        /// @return the output of the PID controller
        float peek(float error);

        /// @brief  resets the integral and last error
        void reset(void);



};

#endif