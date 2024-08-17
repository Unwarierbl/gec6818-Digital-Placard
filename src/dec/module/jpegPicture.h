#ifndef JPEG_PICTURE_H
#define JPEG_PICTURE_H

#include "decTypes.h"


struct jpegPicture
{
    vec2  center_cord;
    float scale;

    char          pic_path[128];
    unsigned int* pic_buffer;


    int image_width;    // 应该由程序自己决定大小
    int image_height;   // 不能初始化

    // 以下函数指针
    // 子类设置路径
    void (*load_pic)(struct jpegPicture* this);
};

void init_jpegPic(struct jpegPicture* jpeg);                   // ok
void remove_jpegPic_direct(struct jpegPicture* remove_jpeg);   // ok

// 注意这里并不是结点
struct jpegPicture* request_jpegPic_direct();   // ok


// 结果保存在 pic_buffer 中
void jpegPic_set_pic_path(struct jpegPicture* jpeg, const char* new_path);   // ok
void jpegPic_load_pic(struct jpegPicture* jpeg);                             // ok


void jpegPic_draw(struct jpegPicture* draw_jpeg);   // ok
// 销毁自身的方法(虚),注意将已申请的内存释放掉


#endif