#ifndef BMP_SLIDE_PIC_LIST_H
#define BMP_SLIDE_PIC_LIST_H

#include "decTypes.h"

struct bmpSlidePicList
{
    int              order;         // ok
    vec2             center_cord;   // ok
    struct list_head list_node;     // ok

    bool need_redraw;   // ok

    int cur_display_order;   // ok

    touchStatusData cur_touch_status_data;
    // touchStatusData pre_touch_status_data;

    char          pic_path[128];
    unsigned int* pic_buffer;

    int image_width;    // 应该由程序自己决定大小
    int image_height;   // 不能初始化
};


void init_bmpSlidePicList_node(struct bmpSlidePicList* bmp_node);   // ok

struct bmpSlidePicList* request_bmpSlidePicList_node_direct();   // ok

struct bmpSlidePicList* find_bmpSlidePicList_node(struct bmpSlidePicList* head_node,   // ok
                                                  int                     find_order);

// void remove_bmpSlidePicList_node_direct(struct bmpSlidePicList* remove_node);
// void remove_all_bmpSlidePicList_node(struct bmpSlidePicList* head_node);
// void destroy_all_bmpSlidePicList_node(struct bmpSlidePicList* head_node);

void insert_bmpSlidePicList_node(struct bmpSlidePicList* head_node,   // ok
                                 struct bmpSlidePicList* insert_node);


void bmpSlidePicList_set_pic_path(struct bmpSlidePicList* bmp_node, const char* new_path);   // ok

void bmpSlidePicList_load_pic(struct bmpSlidePicList* bmp_node);   // ok

void bmpSlidePicList_draw(struct bmpSlidePicList* head_node);

void bmpSlidePicList_analyze_touch(struct bmpSlidePicList* head_node,   // ok
                                   touchStatusData         touchScreen_data);



#endif
