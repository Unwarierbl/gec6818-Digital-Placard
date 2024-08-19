#include "page/shutdownPage.h"

#include "module/bmpGifList.h"
#include "module/jpegPicture.h"
#include "device/frameBuffer.h"
#include "painter.h"



#include <time.h>
#include <sys/timeb.h>

static enum PAGE* store_page_order_addr;



struct shutdownPage* request_shutdownPage_direct()
{
    struct shutdownPage* new_page = (struct shutdownPage*)malloc(sizeof(struct shutdownPage));
    init_shutdownPage(new_page);

    return new_page;
}



void init_shutdownPage(struct shutdownPage* page)
{
    page->gif_thank_head_node                = request_bmpGifList_node_direct();
    page->gif_thank_head_node->center_cord.x = 0;
    page->gif_thank_head_node->center_cord.y = 0;
    page->gif_thank_head_node->animFPS       = 1.0f / (FRAME_DELTA_TIME * 4.0f);
    for (int i = 1; i <= 6; i++) {
        char temp_path_str[128];
        memset(temp_path_str, 0, sizeof(temp_path_str));

        struct bmpGifList* temp_bmp_node = request_bmpGifList_node_direct();
        if (i < 10) {
            sprintf(temp_path_str, "res/loveU/loveU_%d.bmp", i);
            bmpGifList_node_set_pic_path(temp_bmp_node, temp_path_str);
        }
        bmpGifList_load_pic(temp_bmp_node);
        insert_bmpGifList_node(page->gif_thank_head_node, temp_bmp_node);
    }
}



void run_shutdownPage(struct shutdownPage* page, enum PAGE* page_order_addr)
{
    // store_page_order_addr = page_order_addr;
    // frameBuffer_set_bg_as_color(Black);
    // frameBuffer_display_frame();
    store_page_order_addr = page_order_addr;

    frameBuffer_set_bg_as_cur_display();

    time_t start_time;
    time_t cur_time;
    double cur_tick_time;

    start_time = time(NULL);
    cur_time   = time(NULL);

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

            cur_time      = time(NULL);
            cur_tick_time = difftime(cur_time, start_time);
            if (cur_tick_time > 4) {
                sleep(1);
                frameBuffer_set_bg_as_color(Black);
                clear_screen();
                frameBuffer_display_frame();
                *store_page_order_addr = NO_PAGE;
            }


            bmpGifList_update(page->gif_thank_head_node, delta_time);
            bmpGifList_draw(page->gif_thank_head_node);

            frameBuffer_display_frame();
        }

    } while (*page_order_addr == SHUTDOWN_PAGE);
}
