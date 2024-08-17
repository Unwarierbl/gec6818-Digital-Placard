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

    page->login_button = request_bmpButton_direct();
    bmpButton_set_press_pic_path(page->login_button, "res/button/login_1.bmp");
    bmpButton_set_release_pic_path(page->login_button, "res/button/login_2.bmp");
    bmpButton_load_pic(page->login_button);
}

void destroy_loginPage(struct loginPage* page)
{
    remove_jpegPic_direct(page->bg_jpeg_picture);
    free(page);
}



void run_loginPage(struct loginPage* page, enum PAGE* page_order)
{
    jpegPic_draw(page->bg_jpeg_picture);
    frameBuffer_display_frame();

    bmpButton_draw(page->login_button);
    frameBuffer_set_bg_as_cur_display();


    struct timeb ts1, ts2;
    time_t       delta_time;
    ftime(&ts1);

    do {
        // touchScreen_listen();
        // touchScreen_update_data();


        ftime(&ts2);
        delta_time = ts2.millitm - ts1.millitm;
        if (delta_time >= FRAME_DELTA_TIME || delta_time < 0) {
            if (delta_time < 0) {
                delta_time = FRAME_DELTA_TIME;
            }
            ts1 = ts2;

            frameBuffer_display_frame();
        }

    } while (*page_order == LOGIN_PAGE);
}
