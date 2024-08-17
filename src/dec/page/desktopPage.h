#ifndef DESKTOP_PAGE_H
#define DESKTOP_PAGE_H

// #include "decTypes.h"

struct jpegPicture;
struct bmpButton;
struct blankButton;


struct desktopPage
{
    struct jpegPicture* bg_jpeg_picture;   // ok
};

void init_desktopPage(struct desktopPage* page);
void destroy_desktopPage(struct desktopPage* page);

void run_desktopPage(struct desktopPage* page, enum PAGE* page_order);


#endif