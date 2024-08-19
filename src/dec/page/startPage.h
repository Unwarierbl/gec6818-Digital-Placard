#ifndef START_PAGE_H
#define START_PAGE_H


#include "decTypes.h"


struct startPage
{
    float running_speed;

    struct jpegPicture* bg_jpeg_picture;

    struct bmpGifList* gif_runBoki_head_node;
};

struct startPage* request_startPage_direct();


void init_startPage(struct startPage* page);
// void destroy_startPage(struct startPage* page);

void run_startPage(struct startPage* page, enum PAGE* page_order_addr);



#endif