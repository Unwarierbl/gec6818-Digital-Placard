#ifndef SIMPLE_BMP_H
#define SIMPLE_BMP_H


#include "decTypes.h"


struct bmpPicture
{
    vec2 center_cord;

    char          pic_path[128];
    unsigned int* pic_buffer;

    int image_width;   // 应该由程序自己决定大小
    int image_height;
};


void init_bmpPicture(struct bmpPicture* bmp);   // ok

struct bmpPicture* request_bmpPicture_direct();                               // ok
void               remove_bmpPicture_direct(struct bmpPicture* remove_bmp);   // ok

void bmpPicture_set_pic_path(struct bmpPicture* bmp, const char* new_path);   // ok

void bmpPicture_load_pic(struct bmpPicture* bmpPicture);   // ok

void bmpPicture_draw(struct bmpPicture* draw_bmp);



#endif