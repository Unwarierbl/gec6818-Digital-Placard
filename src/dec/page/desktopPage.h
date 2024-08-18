#ifndef DESKTOP_PAGE_H
#define DESKTOP_PAGE_H

// #include "decTypes.h"

struct jpegPicture;
struct bmpPicture;
struct bmpButton;
struct blankButton;
struct bmpTimeDisplayer;
struct bmpGifList;
struct bmpSlidePicList;

enum PAGE;


struct desktopPage
{
    struct jpegPicture* bg_jpeg_picture;   // ok

    struct bmpPicture* bg_bmp_weather_1;   // ok
    struct bmpPicture* bg_bmp_weather_2;   // ok

    struct bmpTimeDisplayer* time_displayer;   // ok

    struct bmpGifList* advert1_head_node;   // ok

    struct bmpSlidePicList* slide_announce_head_node;
};

struct desktopPage* request_desktopPage_direct();
// struct desktopPage* request_desktopPage();


void init_desktopPage(struct desktopPage* page);
void destroy_desktopPage(struct desktopPage* page);

void run_desktopPage(struct desktopPage* page, enum PAGE* page_order_addr);


#endif