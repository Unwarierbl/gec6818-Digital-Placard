#include "page/desktopPage.h"



#include "decTypes.h"
#include "device/touchScreen.h"
#include "device/frameBuffer.h"
#include "painter.h"

#include "module/jpegPicture.h"
#include "module/bmpButton.h"
#include "module/blankButton.h"
#include "module/bmpTimeDisplayer.h"
#include "module/bmpGifList.h"
#include "module/bmpPicture.h"
#include "module/bmpSlidePicList.h"
#include "module/blankSlider.h"

#include "accountInfo.h"


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <time.h>
#include <sys/timeb.h>


static int advert_choice;

static enum PAGE* store_page_order_addr;


struct desktopPage* request_desktopPage_direct()
{
    struct desktopPage* new_page = (struct desktopPage*)malloc(sizeof(struct desktopPage));
    // init_desktopPage(new_page);

    return new_page;
}



void init_desktopPage(struct desktopPage* page)
{
    advert_choice = 0;

    page->bg_jpeg_picture = request_jpegPic_direct();
    jpegPic_set_pic_path(page->bg_jpeg_picture, "res/bg_2.jpg");
    jpegPic_load_pic(page->bg_jpeg_picture);

    page->bg_bmp_weather_1                = request_bmpPicture_direct();
    page->bg_bmp_weather_1->center_cord.x = -280;
    page->bg_bmp_weather_1->center_cord.y = 90;
    bmpPicture_set_pic_path(page->bg_bmp_weather_1, "res/weather_font/weather_0.bmp");
    bmpPicture_load_pic(page->bg_bmp_weather_1);


    page->bg_bmp_weather_2                = request_bmpPicture_direct();
    page->bg_bmp_weather_2->center_cord.x = -250;
    page->bg_bmp_weather_2->center_cord.y = 180;
    bmpPicture_set_pic_path(page->bg_bmp_weather_2, "res/weather_font/weather_4.bmp");
    bmpPicture_load_pic(page->bg_bmp_weather_2);

    page->time_displayer                = request_bmpTimeDisplayer_direct();
    page->time_displayer->center_cord.x = 100;
    page->time_displayer->center_cord.y = 150;

    page->advert_head_node[0]                = request_bmpGifList_node_direct();
    page->advert_head_node[0]->center_cord.x = -200;
    page->advert_head_node[0]->center_cord.y = -100;
    page->advert_head_node[0]->animFPS       = 1.0f / (FRAME_DELTA_TIME * 3);
    for (int i = 0; i <= 186; i++) {
        char temp_path_str[128];
        memset(temp_path_str, 0, sizeof(temp_path_str));

        struct bmpGifList* temp_bmp_node = request_bmpGifList_node_direct();
        if (i < 10) {
            sprintf(temp_path_str, "res/ad_1/ad_1__000%d_图层 %d.bmp", i, 187 - i);
            bmpGifList_node_set_pic_path(temp_bmp_node, temp_path_str);
        }
        else if (i < 100) {
            sprintf(temp_path_str, "res/ad_1/ad_1__00%d_图层 %d.bmp", i, 187 - i);
            bmpGifList_node_set_pic_path(temp_bmp_node, temp_path_str);
        }
        else {
            sprintf(temp_path_str, "res/ad_1/ad_1__0%d_图层 %d.bmp", i, 187 - i);
            bmpGifList_node_set_pic_path(temp_bmp_node, temp_path_str);
        }

        bmpGifList_load_pic(temp_bmp_node);
        insert_bmpGifList_node(page->advert_head_node[0], temp_bmp_node);
    }

    page->advert_head_node[1]                = request_bmpGifList_node_direct();
    page->advert_head_node[1]->center_cord.x = -200;
    page->advert_head_node[1]->center_cord.y = -100;
    page->advert_head_node[1]->animFPS       = 1.0f / (FRAME_DELTA_TIME * 3);
    for (int i = 0; i <= 187; i++) {
        char temp_path_str[128];
        memset(temp_path_str, 0, sizeof(temp_path_str));

        struct bmpGifList* temp_bmp_node = request_bmpGifList_node_direct();
        if (i < 10) {
            sprintf(temp_path_str, "res/ad_2/ad_2__000%d_图层 %d.bmp", i, 188 - i);
            bmpGifList_node_set_pic_path(temp_bmp_node, temp_path_str);
        }
        else if (i < 100) {
            sprintf(temp_path_str, "res/ad_2/ad_2__00%d_图层 %d.bmp", i, 188 - i);
            bmpGifList_node_set_pic_path(temp_bmp_node, temp_path_str);
        }
        else {
            sprintf(temp_path_str, "res/ad_2/ad_2__0%d_图层 %d.bmp", i, 188 - i);
            bmpGifList_node_set_pic_path(temp_bmp_node, temp_path_str);
        }

        bmpGifList_load_pic(temp_bmp_node);
        insert_bmpGifList_node(page->advert_head_node[1], temp_bmp_node);
    }

    page->advert_slider                = request_blankSlider_direct();
    page->advert_slider->center_cord.x = -200;
    page->advert_slider->center_cord.y = -100;
    page->advert_slider->image_width   = 332;
    page->advert_slider->image_height  = 200;
    page->advert_slider->handle_slide  = advert_slider_handle_slide;


    page->slide_announce_head_node                = request_bmpSlidePicList_node_direct();
    page->slide_announce_head_node->center_cord.x = 200;
    page->slide_announce_head_node->center_cord.y = -100;
    for (int i = 1; i <= 5; i++) {
        char temp_path_str[128];
        memset(temp_path_str, 0, sizeof(temp_path_str));

        struct bmpSlidePicList* temp_slideBmp_node = request_bmpSlidePicList_node_direct();

        sprintf(temp_path_str, "res/album/album_%d.bmp", i);
        bmpSlidePicList_set_pic_path(temp_slideBmp_node, temp_path_str);

        bmpSlidePicList_load_pic(temp_slideBmp_node);
        insert_bmpSlidePicList_node(page->slide_announce_head_node, temp_slideBmp_node);
    }
}

void destroy_desktopPage(struct desktopPage* page)
{
    free(page);
}


void run_desktopPage(struct desktopPage* page, enum PAGE* page_order_addr)
{
    store_page_order_addr = page_order_addr;

    jpegPic_draw(page->bg_jpeg_picture);
    frameBuffer_display_frame();
    frameBuffer_set_bg_as_cur_display();

    bmpPicture_draw(page->bg_bmp_weather_1);
    frameBuffer_display_frame();
    frameBuffer_set_bg_as_cur_display();
    bmpPicture_draw(page->bg_bmp_weather_2);
    frameBuffer_display_frame();
    frameBuffer_set_bg_as_cur_display();



    struct timeb ts1, ts2;
    time_t       delta_time;
    ftime(&ts1);

    do {

        ftime(&ts2);
        delta_time = ts2.millitm - ts1.millitm;
        if (delta_time >= FRAME_DELTA_TIME || delta_time < 0) {
            if (delta_time < 0) {
                delta_time = FRAME_DELTA_TIME;
            }
            ts1 = ts2;

            touchScreen_listen();
            touchScreen_update_data();


            bmpTimeDisplayer_update(page->time_displayer, delta_time);
            bmpTimeDisplayer_draw(page->time_displayer);

            blankSlider_analyze_touch(page->advert_slider, get_touch_status_data());

            bmpGifList_update(page->advert_head_node[advert_choice], delta_time);
            bmpGifList_draw(page->advert_head_node[advert_choice]);

            bmpSlidePicList_analyze_touch(page->slide_announce_head_node, get_touch_status_data());
            bmpSlidePicList_draw(page->slide_announce_head_node);
        }
        frameBuffer_display_frame();
    } while (*page_order_addr == DESKTOP_PAGE);
}


void advert_slider_handle_slide(struct blankSlider* slider, touchStatusData touchScreen_data)
{
    switch (slider->cur_touch_status_data.slide_status) {
    case SLIDE_LEFT:
        advert_choice -= 1;
        if (advert_choice < 0) {
            advert_choice = 1;
        }
        break;

    case SLIDE_RIGHT:
        advert_choice += 1;
        if (advert_choice > 1) {
            advert_choice = 0;
        }
        break;
        ;
    default: break;
    }
}
