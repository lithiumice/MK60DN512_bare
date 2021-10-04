#include "display_ui.h"
#include "main.h"
#include "string"
#include "myOled.h"

using namespace std;

int16 refresh_lcd_signal = 0;

std::string ui_type;
ui_modify_x x_1;

ui_option_t ui_option;

ui_title_x ui_title_y[] = {
    {show_image_i, "show image", Showimage, 0},
    {show_param_i, "show param", ui_param_fresh, 0},
    {change_param_i, "show modify", ui_modify_fresh, 0},
};

ui_modify_x ui_modify_y[] = {
    {0, "adc_l", &iodev.adc_l, "uint16", 0, 1, 1},
    {0, "adc_r", &iodev.adc_r, "uint16", 0, 1, 1},
    {0, "enc1", &enc1.speed, "float", 0, 1, 1},
    {0, "enc2", &enc2.speed, "float", 0, 1, 1},
};

ui_param_x ui_param_y[] = {
    PU(cpu_usage, iodev.cpu_usage, float),
    PU(test, iodev.test, float),
    PU(enc1, enc1.speed, float),
    PU(enc2, enc2.speed, float),
    PU(pitch, imu.pitch_angle, float),
    PU(yaw_angle, imu.yaw_angle, float),
    PU(roll_angle, imu.roll_angle, float),
    PU(adc_l, iodev.adc_l, uint16),
    PU(adc_r, iodev.adc_r, uint16),

};

void ui_title_fresh()
{
    if (refresh_lcd_signal)
    {
        MyOLED_Fill(0);
        refresh_lcd_signal = 0;
    }

    if (0 == ui_option.level)
    {
        int ui_index = 0;
        int start_pos = UI_LIST_MAX_LEN * ui_option.current_title_page;
        int end_pos = (UI_TITLE_LEN - start_pos) > UI_LIST_MAX_LEN ? UI_LIST_MAX_LEN : (UI_TITLE_LEN - start_pos);

        for (int i = start_pos; i < start_pos + end_pos; i++)
        {
            ui_title_x x_1 = ui_title_y[i];
            std::string ui_str = "   " + (std::string)x_1.title;

            if (ui_index == (ui_option.cursor_level0 % UI_LIST_MAX_LEN))
            {
                ui_str[1] = '>';
            }
            else
            {
                ui_str[1] = ' ';
            }
            MyOLED_P6x8Str(0, ui_index, (uint8 *)ui_str.data());
            ui_index++;
        }
    }
    else if (1 == ui_option.level)
    {
        (*(ui_title_y[ui_option.cursor_level0].current_func))();
    }
}

void ui_param_fresh()
{
    if (refresh_lcd_signal)
    {
        MyOLED_Fill(0);
        refresh_lcd_signal = 0;
    }

    int ui_index = 0;
    int start_pos = UI_LIST_MAX_LEN * ui_option.current_param_page;
    int end_pos = (UI_PARAM_LEN - start_pos) > UI_LIST_MAX_LEN ? UI_LIST_MAX_LEN : (UI_PARAM_LEN - start_pos);

    for (int i = start_pos; i < start_pos + end_pos; i++)
    {
        ui_param_x x_1 = ui_param_y[i];
        std::string ui_str = x_1.param_title;
        std::string ui_type = x_1.param_type;

        if (ui_type == "float")
        {
            TS(ui_index, float, x_1.param_addr);
        }
        else if (ui_type == "int")
        {
            TS(ui_index, int, x_1.param_addr);
        }
        else if (ui_type == "int16")
        {
            TS(ui_index, int16, x_1.param_addr);
        }
        else if (ui_type == "uint8")
        {
            TS(ui_index, uint8, x_1.param_addr);
        }
        else if (ui_type == "uint16")
        {
            TS(ui_index, uint16, x_1.param_addr);
        }

        else if (ui_type == "uint64")
        {
            TS(ui_index, uint64, x_1.param_addr);
        }

        ui_index++;
    }
}

void ui_modify_fresh()
{
    if (refresh_lcd_signal)
    {
        MyOLED_Fill(0);
        refresh_lcd_signal = 0;
    }

    int ui_index = 0;
    int start_pos = UI_LIST_MAX_LEN * ui_option.current_modify_page;
    int end_pos = (UI_MODIFY_LEN - start_pos) > UI_LIST_MAX_LEN ? UI_LIST_MAX_LEN : (UI_MODIFY_LEN - start_pos);

    for (int i = start_pos; i < start_pos + end_pos; i++)
    {
        ui_modify_x x_1 = ui_modify_y[i];
        std::string ui_name = x_1.num_name;
        std::string ui_type = x_1.num_type;
        std::string ui_str;

        if (ui_type == "int16")
        {
            int16 a = *(int16 *)(x_1.num_addr);
            ui_str = "  " + ("(" + std::to_string(a) + ")") + ui_name;
        }
        else if (ui_type == "float")
        {
            int16 a = *(float *)(x_1.num_addr);
            ui_str = "  " + ("(" + std::to_string(a) + ")") + ui_name;
        }
        else if (ui_type == "uint16")
        {
            int16 a = *(uint16 *)(x_1.num_addr);
            ui_str = "  " + ("(" + std::to_string(a) + ")") + ui_name;
        }
        else if (ui_type == "int")
        {
            int16 a = *(int *)(x_1.num_addr);
            ui_str = "  " + ("(" + std::to_string(a) + ")") + ui_name;
        }

        if (ui_index == (ui_option.cursor_level1 % UI_LIST_MAX_LEN))
        {
            ui_str[1] = '>';
        }
        else
        {
            ui_str[1] = ' ';
        }
        MyOLED_P6x8Str(0, ui_index, (uint8 *)ui_str.data());
        ui_index++;
    }
}
