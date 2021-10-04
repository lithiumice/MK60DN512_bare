#ifndef __MOTOR_H__
#define __MOTOR_H__

#include "headfile.h"
using namespace std;

#define MOTOR_PWM_FREQ 17000

class motor
{
public:
    int index;

    motor() {}

    motor(int index) : index(index) {}

    void init()
    {
        if (1 == index)
        {
            ftm_pwm_init(ftm0, ftm_ch4, MOTOR_PWM_FREQ, 0);
            ftm_pwm_init(ftm0, ftm_ch5, MOTOR_PWM_FREQ, 0);
        }
        else
        {
            ftm_pwm_init(ftm0, ftm_ch6, MOTOR_PWM_FREQ, 0);
            ftm_pwm_init(ftm0, ftm_ch7, MOTOR_PWM_FREQ, 0);
        }
    }

    void run(int thousands)
    {
        __Limit_Both(thousands, 1000);
        thousands *= 50;

        if (1 == index)
        {
            if (thousands >= 0)
            {
                ftm_pwm_duty(ftm0, ftm_ch4, thousands / 10);
                ftm_pwm_duty(ftm0, ftm_ch5, 0);
            }
            else
            {
                thousands = -thousands;

                ftm_pwm_duty(ftm0, ftm_ch4, 0);
                ftm_pwm_duty(ftm0, ftm_ch5, thousands / 10);
            }
        }
        else
        {
            if (thousands >= 0)
            {
                ftm_pwm_duty(ftm0, ftm_ch6, thousands / 10);
                ftm_pwm_duty(ftm0, ftm_ch7, 0);
            }
            else
            {
                thousands = -thousands;

                ftm_pwm_duty(ftm0, ftm_ch6, 0);
                ftm_pwm_duty(ftm0, ftm_ch7, thousands / 10);
            }
        }
    }
};

#endif
