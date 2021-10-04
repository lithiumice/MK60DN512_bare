#include "inoutdev.h"
#include "main.h"
int16 beetime = 0;
inoutdev iodev;

void inoutdev::beep(int num)
{
    beetime = 50;
}
extern "C"
{
    void bee_init(void)
    {
        gpio_init(A26, GPO, 1);
    }

    void bee_Set(int num)
    {
        gpio_set(A26, num);
    }

    void key_init(void)
    {
        gpio_init(KEY_S1, GPI, 1);
        gpio_init(KEY_S2, GPI, 1);
        gpio_init(KEY_S3, GPI, 1);
        gpio_init(KEY_S4, GPI, 1);
        gpio_init(KEY_S5, GPI, 1);
        gpio_init(KEY_S6, GPI, 1);
        gpio_init(KEY_S7, GPI, 1);
        port_init(KEY_S1, IRQ_FALLING | PF | ALT1 | PULLUP);
        port_init(KEY_S2, IRQ_FALLING | PF | ALT1 | PULLUP);
        port_init(KEY_S3, IRQ_FALLING | PF | ALT1 | PULLUP);
        port_init(KEY_S4, IRQ_FALLING | PF | ALT1 | PULLUP);
        port_init(KEY_S5, IRQ_FALLING | PF | ALT1 | PULLUP);
        port_init(KEY_S6, IRQ_FALLING | PF | ALT1 | PULLUP);
        port_init(KEY_S7, IRQ_FALLING | PF | ALT1 | PULLUP);
        set_irq_priority(PORTD_IRQn, 4);
        enable_irq(PORTD_IRQn);
    }

    // void clear_isr()
    // {
    //     PORTD->ISFR = 0xffffffff;
    // }
    // extern unsigned int taskNum[5];

    void task_handle()
    {
        // for (int i = 0; i < __Sizeof(taskNum); i++)
        //     if (taskNum[i])
        //         taskNum[i]--;
        if (beetime > 0)
        {
            beetime--;
            bee_Set(BEE_HIGH);
        }
        else
        {
            bee_Set(BEE_LOW);
        }
    }

    int is_key_on(PTX_n key_num)
    {
        int cnt = 0;
        for (int i = 0; i < 500; i++)
        {
            if (gpio_get(key_num) == 0)
            {
                cnt++;
            }
        }

        if (cnt < 490)
        {
            return 0;
        }
        else
        {
            return 1;
        }
    }

    void key_scan()
    {
        //S1
        if (is_key_on(KEY_S1))
        {
            beetime = 50;
            refresh_lcd_signal = 1;

            if (0 == ui_option.level)
            {
                if (0 == ui_title_y[ui_option.cursor_level0].func_type)
                    ui_option.level = 1;
                else
                    (*(ui_title_y[ui_option.cursor_level0].current_func))();
            }
            else if (1 == ui_option.level)
            {
                ui_option.level = 0;
            }
        }

        //S2
        if (is_key_on(KEY_S2))
        {
            beetime = 50;
            refresh_lcd_signal = 1;

            if (0 == ui_option.level)
            {
                __ValuePlus(ui_option.cursor_level0, 1, 0, UI_TITLE_LEN - 1);
                ui_option.current_title_page = ui_option.cursor_level0 / UI_LIST_MAX_LEN;
            }
            else if (1 == ui_option.level)
            {
                switch (ui_option.cursor_level0)
                {
                }
            }
        }

        //S3
        if (is_key_on(KEY_S3))
        {
            beetime = 50;
            refresh_lcd_signal = 1;

            if (0 == ui_option.level)
            {
                __ValuePlus(ui_option.cursor_level0, -1, 0, UI_TITLE_LEN - 1);
                ui_option.current_title_page = ui_option.cursor_level0 / UI_LIST_MAX_LEN;
            }
            else if (1 == ui_option.level)
            {
                switch (ui_option.cursor_level0)
                {
                }
            }
        }

        //S4
        if (is_key_on(KEY_S4))
        {
            beetime = 50;
            refresh_lcd_signal = 1;
        }

        //S5
        if (is_key_on(KEY_S5))
        {
            beetime = 50;
            refresh_lcd_signal = 1;

            if (1 == ui_option.level)
            {
                switch (ui_option.cursor_level0)
                {
                case change_param_i:
                    ui_modify_x x_1 = ui_modify_y[ui_option.cursor_level1];
                    if (x_1.num_type == "int16")
                        M_1(*(int16 *)(x_1.num_addr), x_1.end);
                    break;
                }
            }
        }

        //S6
        if (is_key_on(KEY_S6))
        {
            beetime = 50;
            refresh_lcd_signal = 1;

            if (1 == ui_option.level)
            {
                switch (ui_option.cursor_level0)
                {
                case change_param_i:
                    ui_modify_x x_1 = ui_modify_y[ui_option.cursor_level1];
                    if (x_1.num_type == "int16")
                        P_1(*(int16 *)(x_1.num_addr), x_1.end);
                    break;
                }
            }
        }

        //S7
        if (is_key_on(KEY_S7))
        {
            beetime = 50;
            refresh_lcd_signal = 1;

            mot1.run(0);
        }

        // clear_isr();
        PORTD->ISFR = 0xffffffff;
    }
}