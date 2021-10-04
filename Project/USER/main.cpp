#include "main.h"
#include "MillisTaskManager.h"

static MillisTaskManager mtmMain;
int ov_choose = OV_PIN16;
float CPU_Usage;
encoder enc1;
encoder enc2;
motor mot1;
motor mot2;
gyro imu;

extern "C"
{
    void imu_update()
    {
        //imu.update();
    }
}

void img_process()
{
    if (ov7725_finish_flag || ov7725_uart_finish_flag)
    {
        // img_extract();

        ov7725_finish_flag = 0;
        ov7725_uart_finish_flag = 0;
    }
}

void report()
{
    printf("%d\n", ov7725_finish_flag);
    uart_putstr(uart5, "hello\n");
    enc1.measure_speed();
    enc2.measure_speed();

    iodev.adc_l = adc_once(ADC1_SE12, ADC_12bit);
    iodev.adc_r = adc_once(ADC1_SE13, ADC_12bit);
    iodev.cpu_usage = mtmMain.GetCPU_Usage();
    iodev.test = systick_getval();
}

int main(void)
{
    DisableInterrupts;
    get_clk();
    MyOLED_Init();
    MyOLED_Fill(0);
    MyOLED_P6x8Str(0, 0, (uint8 *)"init...");
    // ov7725_uart_init();
    // Icm20602_init_spi();

    // uart_init(uart0, 115200);
    // uart_init(uart5, 115200);

    adc_init(ADC1_SE12);
    adc_init(ADC1_SE13);

    //bee_init();
    key_init();
    Priority_Init();

    enc1 = encoder(1);
    enc2 = encoder(2);
    enc1.init();
    enc2.init();

    mot1 = motor(1);
    mot2 = motor(2);
    mot1.init();
    mot2.init();

    EnableInterrupts;
    systick_delay_ms(500);

    mtmMain.Register(ui_title_fresh, 40);
    mtmMain.Register(img_process, 20);
    mtmMain.Register(report, 100);

    while (1)
    {
        mtmMain.Running(millis());
    }
}
