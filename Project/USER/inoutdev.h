#ifndef __KEY_H_
#define __KEY_H_
#include "headfile.h"
#ifdef __cplusplus
extern "C"
{
#endif

#define BEE_HIGH 1
#define BEE_LOW 0

#define KEY_S1 D0
#define KEY_S2 D1
#define KEY_S3 D2
#define KEY_S4 D3
#define KEY_S5 D8
#define KEY_S6 D9
#define KEY_S7 D10

    void key_init(void);
    void IsKeyExit(uint8 key_num);
    void bee_init(void);
    void bee_Set(int num);
    void key_scan();

    extern int16 beetime;
#ifdef __cplusplus
}
#endif

class inoutdev
{
public:
    uint16 adc_l;
    uint16 adc_r;

    float cpu_usage;
    int test;

    void beep(int num);
    void lbeep(int num);
};
extern inoutdev iodev;

#endif
