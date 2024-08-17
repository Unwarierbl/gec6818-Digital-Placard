#include "page/loginPage.h"

#include "decTypes.h"
#include "device/touchScreen.h"
#include "device/frameBuffer.h"

#include "module/jpegPicture.h"
#include "module/bmpButton.h"


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <time.h>
#include <sys/timeb.h>

static enum PAGE* store_page_order_addr;

struct loginPage* request_loginPage_direct()
{
    struct loginPage* new_page = (struct loginPage*)malloc(sizeof(struct loginPage));
    init_loginPage(new_page);

    return new_page;
}



void init_loginPage(struct loginPage* page)
{
    page->bg_jpeg_picture = request_jpegPic_direct();
    jpegPic_set_pic_path(page->bg_jpeg_picture, "res/bg_1.jpg");
    jpegPic_load_pic(page->bg_jpeg_picture);

    page->login_button                 = request_bmpButton_direct();
    page->login_button->handle_release = loginButton_action;
    bmpButton_set_release_pic_path(page->login_button, "res/button/login_1.bmp");
    bmpButton_set_press_pic_path(page->login_button, "res/button/login_2.bmp");
    bmpButton_load_pic(page->login_button);
}

void destroy_loginPage(struct loginPage* page)
{
    remove_jpegPic_direct(page->bg_jpeg_picture);
    free(page);
}



void run_loginPage(struct loginPage* page, enum PAGE* page_order_addr)
{
    store_page_order_addr = page_order_addr;

    jpegPic_draw(page->bg_jpeg_picture);
    frameBuffer_display_frame();

    // bmpButton_draw(page->login_button);
    // frameBuffer_set_bg_as_cur_display();


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

            bmpButton_analyze_touch(page->login_button, get_touch_status_data());
            bmpButton_draw(page->login_button);

            // touchStatusData temp_data;
            // temp_data = get_touch_status_data();
            // printf("touch status=%d,cur_touch_cord=(%d,%d)\n",
            //        temp_data.touch_status,
            //        temp_data.cur_touch_cord.x,
            //        temp_data.cur_touch_cord.y);



            frameBuffer_display_frame();
        }

    } while (*page_order_addr == LOGIN_PAGE);
}


void loginButton_action()
{
    *store_page_order_addr = NO_PAGE;
}
