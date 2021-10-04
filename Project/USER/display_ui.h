#ifndef _DIS_UI_H_
#define _DIS_UI_H_
#include "headfile.h"
#ifdef __cplusplus
extern "C"
{
#endif

#define FS(name, addr)                                                                              \
    {                                                                                               \
        MyOLED_P6x8Str(0, ui_option.ui_detail_index++, (#name ": " + std::to_string(addr)).data()); \
    }

#define TS(index, type, addr)                                                                                      \
    {                                                                                                              \
        MyOLED_P6x8Str(0, index, (uint8 *)(" " + ui_str + ": " + std::to_string(*(type *)(addr)) + "   ").data()); \
    }

#define PU(name, addr, type)   \
    {                          \
        0, #name, &addr, #type \
    }

#define PU_S(addr, type)       \
    {                          \
        0, #addr, &addr, #type \
    }

#define P_1(addr, end)       \
    {                        \
        (addr) = (addr) + 1; \
        if ((addr) > end)    \
            (addr) = 0;      \
    }

#define M_1(addr, end)         \
    {                          \
        if (addr == 0)         \
            (addr) = end;      \
        else                   \
            (addr) = (addr)-1; \
    }

    enum ui_title_index
    {
        show_image_i,
        show_param_i,
        change_param_i,
    };

    typedef struct
    {
        int ui_detail_index = 0;
        int current_title_page = 0;
        int current_param_page = 0;
        int current_modify_page = 0;
        int cursor_level0 = 0;
        int cursor_level1 = 0;
        int line_pos = 110;
        int level = 0;
    } ui_option_t;

    typedef struct
    {
        int index;
        char *title;
        void (*current_func)();
        uint8 func_type;
        //0:loop; 1: once
    } ui_title_x;

    typedef struct
    {
        int index;
        char *param_title;
        void *param_addr;
        char *param_type;
    } ui_param_x;

    typedef struct
    {
        int index;
        char *num_name;
        void *num_addr;
        char *num_type;
        int start;
        int step;
        int end;
    } ui_modify_x;

#define UI_LIST_MAX_LEN 8
#define UI_TITLE_LEN (int((sizeof(ui_title_y) / sizeof(ui_title_x))))
#define UI_TITLE_MAX_PAGE (int((UI_TITLE_LEN / UI_LIST_MAX_LEN)) + 1)
#define UI_PARAM_LEN (int((sizeof(ui_param_y) / sizeof(ui_param_x))))
#define UI_PARAM_MAX_PAGE (int((UI_PARAM_LEN / UI_LIST_MAX_LEN)) + 1)
#define UI_MODIFY_LEN (int((sizeof(ui_modify_y) / sizeof(ui_modify_x))))
#define UI_MODIFY_MAX_PAGE (int((UI_MODIFY_LEN / UI_LIST_MAX_LEN)) + 1)

    void ui_title_fresh();
    void upload_zf_gray();
    void upload_zf_bin();
    void ui_param_fresh();
    void ui_modify_fresh();
    void show_image();
    void send_reset_sig();
    void send_refresh_sig();
    void show_line();
    void tweak_bright();
    void tweak_threshold();
    void tweak_auto_exp();
    void show_cam();

    extern ui_option_t ui_option;
    extern ui_title_x ui_title_y[3];
    extern ui_modify_x ui_modify_y[4];
    extern ui_param_x ui_param_y[9];
    extern int16 refresh_lcd_signal;
#ifdef __cplusplus
}
#endif

#endif