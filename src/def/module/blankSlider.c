#include "module/blankSlider.h"



void init_blankSlider(struct blankSlider* slider)
{
    slider->center_cord.x = 0;
    slider->center_cord.y = 0;

    slider->image_width  = 0;
    slider->image_height = 0;

    slider->handle_slide = NULL;
}


struct blankSlider* request_blankSlider_direct()
{
    struct blankSlider* new_slider = (struct blankSlider*)malloc(sizeof(struct blankSlider));
    init_blankSlider(new_slider);

    return new_slider;
}


void remove_blankSlider_direct(struct blankSlider* remove_slider)
{
    free(remove_slider);
}


void blankSlider_analyze_touch(struct blankSlider* slider, touchStatusData touchScreen_data)
{
    vec2 touch_cord = touchScreen_data.cur_touch_cord;

    switch (touchScreen_data.touch_status) {
    case PRESS:
        if (touch_cord.x >= slider->center_cord.x - slider->image_width / 2 &&
            touch_cord.x <= slider->center_cord.x + slider->image_width / 2 &&
            touch_cord.y >= slider->center_cord.y - slider->image_height / 2 &&
            touch_cord.y <= slider->center_cord.y + slider->image_height / 2) {

            slider->cur_touch_status_data.touch_status = PRESS;
        }
        break;
    case SLIDE:
        if (slider->cur_touch_status_data.touch_status == PRESS) {
            if ((touchScreen_data.slide_status & SLIDE_LEFT) == SLIDE_LEFT) {
                slider->cur_touch_status_data.touch_status = SLIDE;
                slider->cur_touch_status_data.slide_status = SLIDE_LEFT;

                if (slider->handle_slide != NULL) {
                    slider->handle_slide(slider, touchScreen_data);
                }
            }
            else if ((touchScreen_data.slide_status & SLIDE_RIGHT) == SLIDE_RIGHT) {
                slider->cur_touch_status_data.touch_status = SLIDE;
                slider->cur_touch_status_data.slide_status = SLIDE_RIGHT;

                if (slider->handle_slide != NULL) {
                    slider->handle_slide(slider, touchScreen_data);
                }
            }
            break;


        case RELEASE:
            if (slider->cur_touch_status_data.touch_status == SLIDE) {
                if (touch_cord.x >= slider->center_cord.x - slider->image_width / 2 &&
                    touch_cord.x <= slider->center_cord.x + slider->image_width / 2 &&
                    touch_cord.y >= slider->center_cord.y - slider->image_height / 2 &&
                    touch_cord.y <= slider->center_cord.y + slider->image_height / 2) {}
            }
            break;
        default: break;
        }
    }
}
