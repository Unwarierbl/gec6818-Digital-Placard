#ifndef DESKTOP_PAGE_H
#define DESKTOP_PAGE_H

#include "decTypes.h"

struct jpegPicture;
struct bmpPicture;
struct bmpButton;
struct blankButton;
struct bmpTimeDisplayer;
struct bmpGifList;
struct bmpSlidePicList;
struct blankSlider;

enum PAGE;


struct desktopPage
{
    struct jpegPicture* bg_jpeg_picture;   // ok

    struct bmpPicture* bg_bmp_weather_1;   // ok
    struct bmpPicture* bg_bmp_weather_2;   // ok

    struct bmpButton* shutdown_button;

    struct bmpTimeDisplayer* time_displayer;   // ok

    struct bmpGifList* advert_head_node[3];   // ok

    struct blankSlider* advert_slider;

    struct bmpSlidePicList* slide_announce_head_node;   // ok
};

struct desktopPage* request_desktopPage_direct();   // ok
// struct desktopPage* request_desktopPage();


void init_desktopPage(struct desktopPage* page);
void destroy_desktopPage(struct desktopPage* page);

void run_desktopPage(struct desktopPage* page, enum PAGE* page_order_addr);

void advert_slider_handle_slide(struct blankSlider* slider, touchStatusData touchScreen_data);

void advert_slider_handle_slide(struct blankSlider* slider, touchStatusData touchScreen_data);

// void shutdown_button_action();

#endif