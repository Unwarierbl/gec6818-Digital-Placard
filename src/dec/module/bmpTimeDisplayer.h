#ifndef BMP_TIME_LCD_H
#define BMP_TIME_LCD_H

#include "decTypes.h"

#include <time.h>


struct bmpTimeDisplayer
{
    vec2 center_cord;

    int whole_width;    // 应该由程序自己决定大小
    int whole_height;   // 不能初始化

    time_t initiate_time;
    time_t start_time;
    time_t cur_time;

    vec2 pic_length[10];

    char  number_pic_path[10][128];
    char* number_pic_buffer[10];
};


struct bmpTimeDisplayer* request_bmpTimeLcd_direct();

void init_bmpTimeLcd(struct bmpTimeDisplayer* displayer, time_t initiate_time);

struct bmpTimeDisplayer* request_bmpTimeLcd_direct();
void                     remove_bmpTimeLcd_direct(struct bmpTimeDisplayer* remove_displayer);

void bmpTimeLcd_load_pic(struct bmpTimeDisplayer* displayer);

void bmpTimeLcd_draw(struct bmpTimeDisplayer* draw_displayer);
void bmpTimeLcd_update(struct bmpTimeDisplayer* draw_displayer, int delta_time);



#endif