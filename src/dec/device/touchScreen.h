#ifndef TOUCH_SCREEN_H
#define TOUCH_SCREEN_H

// 触摸屏功能:
//      检测输入坐标
//      更新触摸状态(所以滑动方向应该不需要)
//      需要判断 slide 和 hold


#include "decTypes.h"



int init_touchScreen_device();      // ok
int destroy_touchScreen_device();   // ok

void touchScreen_listen();   // ok

void touchScreen_update_data();   // ok
// 滑动返回1,否则返回0
int judge_hold_or_slide();   // ok

touchStatusData get_touch_status_data();   // ok

#endif