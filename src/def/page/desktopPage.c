#include "page/desktopPage.h"



#include "decTypes.h"
#include "device/touchScreen.h"
#include "device/frameBuffer.h"
#include "painter.h"

#include "module/jpegPicture.h"
#include "module/bmpButton.h"
#include "module/blankButton.h"
#include "module/bmpTimeDisplayer.h"

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
}

void destroy_desktopPage(struct desktopPage* page)
{
    free(page);
}


void run_desktopPage(struct desktopPage* page, enum PAGE* page_order_addr)
{
    store_page_order_addr = page_order_addr;

    jpegPic_draw(page->bg_jpeg_picture);

    // painter_draw_ARGB_pic(page->time_displayer->number_pic_buffer[2], 0, 0, 80, 100);



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

            // printf("desktop running\n");
        }
        frameBuffer_display_frame();
    } while (*page_order_addr == DESKTOP_PAGE);
}
