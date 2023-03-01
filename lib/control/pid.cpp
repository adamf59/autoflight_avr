#include <control.h>
#include <AF_Math/AF_Math.h>
#include <stdlib.h>


PID::PID(float kp, float ki, float kd, float bias, float max_output, float min_output, float max_integral, float min_integral, uint8_t var_flags, const char* prefix) {

    _kp = AF_Float("", kp, var_flags); // TODO: add prefix
    _ki = AF_Float("", ki, var_flags); // TODO: add prefix
    _kd = AF_Float("", kd, var_flags); // TODO: add prefix
    _bias = AF_Float("", bias, var_flags);
    _max_output = max_output;
    _min_output = min_output;
    _max_integral = max_integral;
    _min_integral = min_integral;
}

float PID::output(float error) {    
    // recompute integral
    _integral += error;
    // clamp between [ _min_integral, _max_integral ]
    _integral = clamp(_integral, _min_integral, _max_integral);
    float delta = error - _last_error;
    _last_error = error;
    return clamp((_kp * error) + (_ki * _integral) + (_kd * delta) + _bias, _min_output, _max_output);
}

float PID::peek(float error) {
    float delta = error - _last_error;
    return clamp((_kp * error) + (_ki * (_integral + error)) + (_kd * delta) + _bias, _min_output, _max_output);
}

void PID::reset(void) {
    _last_error = 0;
    _integral = 0;
}