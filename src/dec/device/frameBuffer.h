#ifndef FRAME_BUFFER_H
#define FRAME_BUFFER_H

// 设置屏幕中心点的坐标为 (0, 0)
// 则其他几个边缘点的坐标为
// 左上 (-400,-240)
// 左下 (-400, 240 - 1)
// 右上 (400 - 1,-240)
// 右下 (400 - 1, 240)

// 变换公式:
//      trans_x = row_x + 400
//      trans_y = row_y + 240


// 本文件只实现显示,不实现绘画

// 要添加透明度支持

// 屏幕刷新过程:
//      绘出背景
//      各组件绘图
//      如果组件绘图需要更新,刷新对应区域
//      display

int  init_frameBuffer_device();      // ok
void destroy_frameBuffer_device();   // ok

unsigned int* get_frameBuffer_cur_buffer();   // ok

void frameBuffer_display_frame();   // ok

void frameBuffer_set_bg_as_color(unsigned int color);   // ok
void frameBuffer_set_bg_as_cur_display();               // ok

void clear_screen();                                            // ok
void clear_screen_range(int x, int y, int width, int height);   // ok

void set_point_RGB_color(int x, int y, unsigned int color);    // ok
void set_point_ARGB_color(int x, int y, unsigned int color);   // ok

#endif