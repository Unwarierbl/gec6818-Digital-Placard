#include "page/startPage.h"


#include "module/jpegPicture.h"
#include "module/bmpGifList.h"
#include "device/frameBuffer.h"
#include "painter.h"

#include <time.h>
#include <sys/timeb.h>

static enum PAGE* store_page_order_addr;

struct startPage* request_startPage_direct()
{
    struct startPage* new_page = (struct startPage*)malloc(sizeof(struct startPage));
    init_startPage(new_page);

    return new_page;
}



void init_startPage(struct startPage* page)
{
    page->running_speed = -4;

    page->bg_jpeg_picture = request_jpegPic_direct();
    jpegPic_set_pic_path(page->bg_jpeg_picture, "res/bg_1.jpg");
    jpegPic_load_pic(page->bg_jpeg_picture);


    page->gif_runBoki_head_node                = request_bmpGifList_node_direct();
    page->gif_runBoki_head_node->center_cord.x = 370;
    page->gif_runBoki_head_node->center_cord.y = 130;
    page->gif_runBoki_head_node->animFPS       = 1.0f / (FRAME_DELTA_TIME * 4.0f);
    for (int i = 0; i <= 7; i++) {
        char temp_path_str[128];
        memset(temp_path_str, 0, sizeof(temp_path_str));

        struct bmpGifList* temp_bmp_node = request_bmpGifList_node_direct();
        if (i < 10) {
            sprintf(temp_path_str, "res/run_boki/run_boki_000%d.bmp", i);
            bmpGifList_node_set_pic_path(temp_bmp_node, temp_path_str);
        }
        bmpGifList_load_pic(temp_bmp_node);
        insert_bmpGifList_node(page->gif_runBoki_head_node, temp_bmp_node);
    }
}


void run_startPage(struct startPage* page, enum PAGE* page_order_addr)
{
    store_page_order_addr = page_order_addr;

    jpegPic_draw(page->bg_jpeg_picture);


    frameBuffer_display_frame();
    frameBuffer_set_bg_as_cur_display();


    struct timeb ts1, ts2;
    time_t       delta_time;
    ftime(&ts1);

    int   start_x    = page->gif_runBoki_head_node->center_cord.x;
    int   pre_cord_x = page->gif_runBoki_head_node->center_cord.x;
    float cur_offset = 0;
    float pre_offset = 0;

    do {

        ftime(&ts2);
        delta_time = ts2.millitm - ts1.millitm;
        if (delta_time >= FRAME_DELTA_TIME || delta_time < 0) {
            if (delta_time < 0) {
                delta_time = FRAME_DELTA_TIME;
            }
            ts1 = ts2;

            if (page->gif_runBoki_head_node->center_cord.x < -450) {
                *store_page_order_addr = LOGIN_PAGE;
            }


            bmpGifList_update(page->gif_runBoki_head_node, delta_time);

            cur_offset += page->running_speed * delta_time / (FRAME_DELTA_TIME);
            pre_cord_x = page->gif_runBoki_head_node->center_cord.x;

            page->gif_runBoki_head_node->center_cord.x = start_x + cur_offset;


            if (page->gif_runBoki_head_node->need_redraw == true) {

                painter_clear_range(pre_cord_x - page->gif_runBoki_head_node->image_width / 2,
                                    page->gif_runBoki_head_node->center_cord.y -
                                        page->gif_runBoki_head_node->image_height / 2,
                                    page->gif_runBoki_head_node->image_width + 10,
                                    page->gif_runBoki_head_node->image_height);


                struct bmpGifList* pos;

                if (page->gif_runBoki_head_node->cur_frame < 1.0f) {
                    page->gif_runBoki_head_node->cur_frame = 1.0f;
                }
                // printf("cur frame=%d\n", head_node->cur_frame);
                pos = find_bmpGifList_node(page->gif_runBoki_head_node,
                                           (int)page->gif_runBoki_head_node->cur_frame);
                if (pos == page->gif_runBoki_head_node) {
                    pos = list_entry(
                        page->gif_runBoki_head_node->list_node.next, struct bmpGifList, list_node);
                }

                painter_draw_ARGB_pic(
                    pos->pic_buffer,
                    page->gif_runBoki_head_node->center_cord.x - pos->image_width / 2,
                    page->gif_runBoki_head_node->center_cord.y - pos->image_height / 2,
                    pos->image_width,
                    pos->image_height);

                page->gif_runBoki_head_node->need_redraw = false;
            }
            frameBuffer_display_frame();
        }
    } while (*page_order_addr == START_PAGE);
}
