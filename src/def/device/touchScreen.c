#include "device/touchScreen.h"
#include "decTypes.h"

#include <fcntl.h>
#include <linux/input.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>


static touchStatusData ts_status_data;

static int                ts_fd;
static ssize_t            rd_size;
static struct input_event ts_data;



int init_touchScreen_device()
{
    ts_fd = open("/dev/input/event0", O_RDONLY);

    if (ts_fd == -1) {
        perror("打开触摸屏失败");
        return -1;
    }

    fcntl(ts_fd, F_SETFL, FNDELAY);   // 设置为非阻塞状态

    return 0;
}

int destroy_touchScreen_device()
{
    return close(ts_fd);
}

void touchScreen_listen()
{
    rd_size = read(ts_fd, &ts_data, sizeof(struct input_event));
    if (rd_size == -1) {
        return;
    }
}

void touchScreen_update_data()
{
    if (rd_size > 0) {
        if (ts_data.type == EV_KEY) {
            if (ts_data.value == 1) {
                ts_status_data.touch_status = PRESS;
                // printf("press\n");
            }
            else {
                ts_status_data.touch_status = RELEASE;   // 这两个状态可能只有一瞬间
                ts_status_data.slide_status = NO_SLIDE;
                // printf("release\n");
            }
        }
        else if (ts_data.type == EV_ABS) {
            if (ts_data.code == ABS_X) {
                ts_status_data.cur_touch_cord.x = ts_data.value * (800.0f / 1024) - 400;
            }
            else if (ts_data.code == ABS_Y) {
                ts_status_data.cur_touch_cord.y = ts_data.value * (480.0f / 600) - 240;
            }


            switch (ts_status_data.touch_status) {
            case PRESS: ts_status_data.press_cord = ts_status_data.cur_touch_cord; break;
            case RELEASE: ts_status_data.release_cord = ts_status_data.cur_touch_cord; break;
            case HOLD:
                if (judge_hold_or_slide()) {
                    ts_status_data.touch_status = SLIDE;
                    update_touch_slide_status();
                }
            case SLIDE: update_touch_slide_status(); break;
            default: break;
            }



            ts_status_data.pre_touch_cord = ts_status_data.cur_touch_cord;
        }
    }
    else {                                       // 并没有检测 rd_size 为负数时的情况
        switch (ts_status_data.touch_status) {   // 无输入时通过前向状态确定当前状态
        case RELEASE:
            ts_status_data.touch_status = NO_TOUCH;
            ts_status_data.slide_status = NO_SLIDE;
            break;
        case PRESS: ts_status_data.touch_status = HOLD; break;

        default: break;
        }
    }
}

int judge_hold_or_slide()
{
    if (abs(ts_status_data.cur_touch_cord.x - ts_status_data.pre_touch_cord.x) >= 3 ||
        abs(ts_status_data.cur_touch_cord.y - ts_status_data.pre_touch_cord.y) >= 3) {
        return 1;
    }
    else {
        return 0;
    }
}

void update_touch_slide_status()
{
    if (abs(ts_status_data.cur_touch_cord.x - ts_status_data.pre_touch_cord.x) >= 3) {
        if (ts_status_data.cur_touch_cord.x > ts_status_data.pre_touch_cord.x) {
            ts_status_data.slide_status &= ~SLIDE_LEFT;
            ts_status_data.slide_status |= SLIDE_RIGHT;
        }
        else {
            ts_status_data.slide_status &= ~SLIDE_RIGHT;
            ts_status_data.slide_status |= SLIDE_LEFT;
        }
    }
    else if (abs(ts_status_data.cur_touch_cord.y - ts_status_data.pre_touch_cord.y) >= 3) {
        if (ts_status_data.cur_touch_cord.y > ts_status_data.pre_touch_cord.y) {
            ts_status_data.slide_status &= ~SLIDE_UP;
            ts_status_data.slide_status |= SLIDE_DOWN;
        }
        else {
            ts_status_data.slide_status &= ~SLIDE_DOWN;
            ts_status_data.slide_status |= SLIDE_UP;
        }
    }
    else {
        ts_status_data.slide_status = NO_SLIDE;
    }
}

touchStatusData get_touch_status_data()
{
    return ts_status_data;
}
