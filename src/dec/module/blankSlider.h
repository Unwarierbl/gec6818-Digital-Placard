#ifndef BLANK_SLIDER_H
#define BLANK_SLIDER_H


#include "decTypes.h"


struct blankSlider
{
    vec2 center_cord;

    int             image_width;   // 应该由程序自己决定大小
    int             image_height;
    touchStatusData cur_touch_status_data;


    void (*handle_slide)(struct blankSlider* slider, touchStatusData touchScreen_data);
};

void init_blankSlider(struct blankSlider* slider);   // ok

struct blankSlider* request_blankSlider_direct();                                   // ok
void                remove_blankSlider_direct(struct blankSlider* remove_slider);   // ok

void blankSlider_analyze_touch(struct blankSlider* slider,
                               touchStatusData     touchScreen_data);   // ok



#endif