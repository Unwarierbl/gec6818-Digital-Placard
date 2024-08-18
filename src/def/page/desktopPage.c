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

#include "accountInfo.h"


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <time.h>
#include <sys/timeb.h>


static enum PAGE* store_page_order_addr;


struct desktopPage* request_desktopPage_direct()
{
    struct desktopPage* new_page = (struct desktopPage*)malloc(sizeof(struct desktopPage));
    // init_desktopPage(new_page);

    return new_page;
}



void init_desktopPage(struct desktopPage* page)
{
    page->bg_jpeg_picture = request_jpegPic_direct();
    jpegPic_set_pic_path(page->bg_jpeg_picture, "res/bg_2.jpg");
    jpegPic_load_pic(page->bg_jpeg_picture);

    page->time_displayer                = request_bmpTimeDisplayer_direct();
    page->time_displayer->center_cord.x = 100;
    page->time_displayer->center_cord.y = 150;

    page->advert1_head_node                = request_bmpGifList_node_direct();
    page->advert1_head_node->center_cord.x = -200;
    page->advert1_head_node->center_cord.y = -100;
    page->advert1_head_node->animFPS       = 1.0f / (FRAME_DELTA_TIME * 3);
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
        insert_bmpGifList_node(page->advert1_head_node, temp_bmp_node);
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

            bmpTimeDisplayer_update(page->time_displayer, delta_time);
            bmpTimeDisplayer_draw(page->time_displayer);

            bmpGifList_update(page->advert1_head_node, delta_time);
            bmpGifList_draw(page->advert1_head_node);
        }
        frameBuffer_display_frame();
    } while (*page_order_addr == DESKTOP_PAGE);
}
