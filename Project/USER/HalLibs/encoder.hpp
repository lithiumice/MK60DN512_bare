#ifndef __ENCODER_H__
#define __ENCODER_H__

#include "main.h"

using namespace std;
#define PULSE_PER_1CM 57.5f

class encoder
{
public:
    int index;
    float rpm = 0;
    float speed = 0;
    float encoder_pul_num = 0;
    float encoder_dis = 0;
    int inverse = 0;

    uint32 last_time = 0;
    uint32 current_time = 0;

    FTMn ftm_i;

    encoder() {}

    encoder(int index) : index(index)
    {
        if (1 == index)
        {
            ftm_i = ftm1;
        }
        else
        {
            ftm_i = ftm2;
        }
    }

    void init()
    {
        ftm_quad_init(ftm_i);
    }

    float measure_speed()
    {
        current_time = millis();

        encoder_pul_num = (float)ftm_quad_get(ftm_i);
        ftm_quad_clean(ftm_i);

        encoder_dis = encoder_pul_num / PULSE_PER_1CM;
        if (inverse)
            encoder_dis = -encoder_dis;

        speed = encoder_dis * 1000 / (current_time - last_time);

        last_time = current_time;
        return speed;
    }
};

#endif
