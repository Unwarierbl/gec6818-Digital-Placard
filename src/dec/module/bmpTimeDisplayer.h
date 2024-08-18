#ifndef BMP_TIME_LCD_H
#define BMP_TIME_LCD_H

#include "decTypes.h"

#include <time.h>

// whole_width = (pix_width * i(2) + font_width +5) * 3 - 5
// whole_height = max_height * 2 + 20

// 公式:    pix_width * i(2) + font_width + 年 *3= whole_width
//          pix_width * i(2) + font_width + 月
//          pix_width * i(2) + font_width   日
//          换行 行间相隔 20 像素  字间相隔 5 像素
//          pix_width * i(2) + font_width   日
//          pix_width * i(2) + font_width   分
//          pix_width * i(2) + font_width   秒

struct bmpTimeDisplayer
{
    vec2 center_cord;

    int whole_width;    // 应该由程序自己决定大小
    int whole_height;   // 不能初始化 ok

    bool need_redraw;

    struct tm initiate_time;   // ok
    struct tm cur_real_time;

    time_t start_time;   // ok
    time_t cur_time;     // ok

    double cur_tick_time;
    double pre_tick_time;


    char          font_pic_path[6][128];   // 年月日时分秒 ok
    unsigned int* font_pic_buffer[6];
    vec2          font_pic_length[6];
    vec2          font_pic_max_length;


    char          number_pic_path[10][128];   // ok
    unsigned int* number_pic_buffer[10];
    vec2          number_pic_length[10];
    vec2          number_pic_max_length;

    int pic_max_height;   // ok
};



void init_bmpTimeDisplayer(struct bmpTimeDisplayer* displayer);   // ok

struct bmpTimeDisplayer* request_bmpTimeDisplayer_direct();                       // ok
void remove_bmpTimeDisplayer_direct(struct bmpTimeDisplayer* remove_displayer);   // ok

void bmpTimeDisplayer_load_pic(struct bmpTimeDisplayer* displayer);   // ok

void bmpTimeDisplayer_update(struct bmpTimeDisplayer* draw_displayer, int delta_time);   // ok
void bmpTimeDisplayer_draw(struct bmpTimeDisplayer* draw_displayer);                     // ok



#endif