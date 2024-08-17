#ifndef BLANK_BUTTON_H
#define BLANK_BUTTON_H


#include "decTypes.h"

struct blankButton
{
    vec2 center_cord;

    enum TOUCH_STATUS cur_press_status;
    enum TOUCH_STATUS pre_draw_press_status;

    int button_width;
    int button_height;

    void (*handle_release)(struct blankButton* button);
};

void init_blankButton(struct blankButton* button);   // ok

struct blankButton* request_blankButton_direct();                                   // ok
void                remove_blankButton_direct(struct blankButton* remove_button);   // ok

void blankButton_analyze_touch(struct blankButton* button,
                               touchStatusData     touchScreen_data);   // ok



#endif