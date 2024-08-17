#ifndef BMP_BUTTON_H
#define BMP_BUTTON_H

#include "decTypes.h"


struct bmpButton
{
    vec2 center_cord;

    enum TOUCH_STATUS cur_press_status;
    enum TOUCH_STATUS pre_draw_press_status;

    char          press_pic_path[128];
    unsigned int* press_pic_buffer;

    char          release_pic_path[128];
    unsigned int* release_pic_buffer;

    int press_image_width;    // 应该由程序自己决定大小
    int press_image_height;   // 不能初始化

    int release_image_width;    // 应该由程序自己决定大小
    int release_image_height;   // 不能初始化

    void (*handle_release)(struct bmpButton* button);
};

void init_bmpButton(struct bmpButton* button);   // ok

struct bmpButton* request_bmpButton_direct();                                 // ok
void              remove_bmpButton_direct(struct bmpButton* remove_button);   // ok

void bmpButton_set_press_pic_path(struct bmpButton* button, const char* new_path);     // ok
void bmpButton_set_release_pic_path(struct bmpButton* button, const char* new_path);   // ok

void bmpButton_load_pic(struct bmpButton* bmpButton);   // ok

void bmpButton_draw(struct bmpButton* draw_button);   // ok

void bmpButton_analyze_touch(struct bmpButton* button, touchStatusData touchScreen_data);   // ok



#endif