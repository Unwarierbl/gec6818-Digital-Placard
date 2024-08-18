#ifndef BMP_GIF_H
#define BMP_GIF_H

#include "decTypes.h"


struct bmpGifList
{
    vec2 center_cord;

    int  order;
    bool need_redraw;

    float cur_frame;
    float animFPS;

    struct list_head list_node;

    char          pic_path[128];
    unsigned int* pic_buffer;

    int image_width;    // 应该由程序自己决定大小
    int image_height;   //
};



void init_bmpGifList_node(struct bmpGifList* bmp_node);   // ok

struct bmpGifList* request_bmpGifList_node_direct();   // ok

struct bmpGifList* find_bmpGifList_node(struct bmpGifList* head_node, int find_order);   // ok

void insert_bmpGifList_node(struct bmpGifList* head_node, struct bmpGifList* insert_node);   // ok

void remove_bmpGifList_node_direct(struct bmpGifList* remove_node);   // ok
void remove_all_bmpGifList_node(struct bmpGifList* head_node);        // ok
void destroy_all_bmpGifList_node(struct bmpGifList* head_node);       // ok

void bmpGifList_node_set_pic_path(struct bmpGifList* bmp_node, char* pic_path);   // ok
void bmpGifList_load_pic(struct bmpGifList* bmp_node);                            // ok

void bmpGifList_draw(struct bmpGifList* head_node);   // ok

void bmpGifList_update(struct bmpGifList* head_node, int delta_time);   // ok


#endif