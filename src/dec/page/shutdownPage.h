#ifndef SHUTDOWN_PAGE_H
#define SHUTDOWN_PAGE_H

#include "decTypes.h"

struct shutdownPage
{
    struct bmpGifList* gif_thank_head_node;
};


struct shutdownPage* request_shutdownPage_direct();
void                 init_shutdownPage(struct shutdownPage* page);
void run_shutdownPage(struct shutdownPage* page, enum PAGE* page_order_addr);   // ok



#endif