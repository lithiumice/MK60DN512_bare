#ifndef __POS_PID__
#define __POS_PID__

#include "headfile.h"
using namespace std;

class pos_pid
{
public:
    int I_limit_flag = 0;
    float I_limit = 0;
    float I_limit_range = 0;

    float kp = 0;
    float ki = 0;
    float kd = 0;

    float error = 0;
    float error_diff = 0;

    float error_history[5];
    float error_diff_history[5];

    float p_error = 0;
    float i_error = 0;
    float d_error = 0;

    float last_out = 0;
    float output = 0;
    float target = 0;
    float intergral = 0;

    float anti_wind_radio = 0.5f;
    float anti_windup_value = 0;

    float maximum = 0;
    float minimum = 0;

    pos_pid() {}

    pos_pid(float kp, float ki, float kd, float maximum, float minimum) : kp(kp), ki(ki), kd(kd), maximum(maximum), minimum(minimum)
    {
        // anti_windup_value = maximum * anti_wind_radio;
        I_limit_range = maximum * 0.5;
        I_limit = maximum * 0.1;
    }

    void set_pid(float kp_p, float ki_p, float kd_p)
    {
        kp = kp_p;
        ki = ki_p;
        kd = kd_p;
    }

    void clear_intergral()
    {
        intergral = 0;
    }

    void update(float current_point, int d_inter = 1)
    {
        error = target - current_point;
        PUSH_PULL(error_history, 5, error);

        error_diff = error_history[0] - error_history[d_inter];
        PUSH_PULL(error_diff_history, 5, error_diff);

        if (current_point > I_limit || current_point < -I_limit)
        {
            I_limit_flag = 0;
        }
        else
        {
            I_limit_flag = 1;
            intergral += ki * error;
            __Limit_Both(intergral, I_limit_range);
        }

        p_error = kp * error;
        i_error = I_limit_flag * intergral;
        d_error = kd * error_diff;

        last_out = p_error + i_error + d_error;
        __Limit_AB(last_out, minimum, maximum);

        output = last_out;
    }
};

#endif