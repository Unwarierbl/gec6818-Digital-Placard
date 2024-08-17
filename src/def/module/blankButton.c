#include "module/blankButton.h"
#include "painter.h"

#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include "decTypes.h"


void init_blankButton(struct blankButton* button)
{
    button->center_cord.x = 0;
    button->center_cord.y = 0;

    button->handle_release = NULL;
}


struct blankButton* request_blankButton_direct()
{
    struct blankButton* new_button = (struct blankButton*)malloc(sizeof(struct blankButton));
    init_blankButton(new_button);

    return new_button;
}

void remove_blankButton_direct(struct blankButton* remove_button)
{
    free(remove_button);
}

void blankButton_analyze_touch(struct blankButton* button, touchStatusData touchScreen_data)
{
    vec2 touch_cord = touchScreen_data.cur_touch_cord;

    switch (touchScreen_data.touch_status) {
    case RELEASE:
        if (touch_cord.x >= button->center_cord.x - button->button_width / 2 &&
            touch_cord.x <= button->center_cord.x + button->button_width / 2 &&
            touch_cord.y >= button->center_cord.y - button->button_height / 2 &&
            touch_cord.y <= button->center_cord.y + button->button_height / 2) {

            button->cur_press_status = RELEASE;
            if (button->handle_release != NULL) {
                button->handle_release(button);
            }
        }
        break;
    default: break;
    }
}
