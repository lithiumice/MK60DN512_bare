#ifndef __MAIN__
#define __MAIN__
#ifdef __cplusplus
// extern "C"
// {
// #endif
#define EXPORT_C(c_func_name, ...) \
    extern "C"                     \
    {                              \
        void #c_func_name ()        \
        {                          \
            __VA_ARGS__;           \
        }                          \
    }
    
#include "headfile.h"
#include "common_macro.h"
#include "inoutdev.h"
#include "nvic.h"
#include "myOled.h"
#include "display_ui.h"
#include "encoder.hpp"
#include "motor.hpp"
#include "quad_num.hpp"
#include "pos_pid.hpp"
#include "gyro.hpp"
#include "icm_spi.h"

extern encoder enc1;
extern encoder enc2;
extern motor mot1;
extern motor mot2;
extern int ov_choose;
extern gyro imu;

int clip(int x, int low, int up);
void LPF_1_db(float hz, float time, float in, float *out);
float fclip(float x, float low, float up);
void push_and_pull(float *buff, int len, float newdata);
void push_int_pull(int *buff, int len, int newdata);

// #ifdef __cplusplus
// }
#endif
#endif