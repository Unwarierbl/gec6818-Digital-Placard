#include "page/loginPage.h"

#include "decTypes.h"
#include "device/touchScreen.h"
#include "device/frameBuffer.h"
#include "painter.h"

#include "module/jpegPicture.h"
#include "module/bmpButton.h"
#include "module/blankButton.h"


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <time.h>
#include <sys/timeb.h>

static int cur_input_focus;

static int press_number;

static int account_numbs;
static int password_numbs;

static enum PAGE* store_page_order_addr;

static int cur_loop_numb;

struct loginPage* request_loginPage_direct()
{
    struct loginPage* new_page = (struct loginPage*)malloc(sizeof(struct loginPage));
    init_loginPage(new_page);

    return new_page;
}



void init_loginPage(struct loginPage* page)
{
    cur_input_focus = 0;

    press_number = 0;

    account_numbs  = 0;
    password_numbs = 0;

    page->bg_jpeg_picture = request_jpegPic_direct();
    jpegPic_set_pic_path(page->bg_jpeg_picture, "res/bg_1.jpg");
    jpegPic_load_pic(page->bg_jpeg_picture);

    page->inputBox_account_button                 = request_blankButton_direct();
    page->inputBox_account_button->center_cord.x  = 50;
    page->inputBox_account_button->center_cord.y  = -75;
    page->inputBox_account_button->button_width   = 250;
    page->inputBox_account_button->button_height  = 50;
    page->inputBox_account_button->handle_release = inputBox_account_action;

    page->inputBox_password_button                 = request_blankButton_direct();
    page->inputBox_password_button->center_cord.x  = 50;
    page->inputBox_password_button->center_cord.y  = 0;
    page->inputBox_password_button->button_width   = 250;
    page->inputBox_password_button->button_height  = 50;
    page->inputBox_password_button->handle_release = inputBox_password_action;


    page->login_button                 = request_bmpButton_direct();
    page->login_button->center_cord.x  = -150;
    page->login_button->center_cord.y  = 180;
    page->login_button->handle_release = loginButton_action;
    bmpButton_set_release_pic_path(page->login_button, "res/button/login_1.bmp");
    bmpButton_set_press_pic_path(page->login_button, "res/button/login_2.bmp");
    bmpButton_load_pic(page->login_button);

    page->regist_button                = request_bmpButton_direct();
    page->regist_button->center_cord.x = 150;
    page->regist_button->center_cord.y = 180;
    // page->login_button->handle_release = loginButton_action;
    bmpButton_set_release_pic_path(page->regist_button, "res/button/regist_1.bmp");
    bmpButton_set_press_pic_path(page->regist_button, "res/button/regist_2.bmp");
    bmpButton_load_pic(page->regist_button);

    page->fork_account_button                 = request_bmpButton_direct();
    page->fork_account_button->center_cord.x  = 220;
    page->fork_account_button->center_cord.y  = -75;
    page->fork_account_button->handle_release = fork_account_action;
    bmpButton_set_release_pic_path(page->fork_account_button, "res/button/fork_1.bmp");
    bmpButton_set_press_pic_path(page->fork_account_button, "res/button/fork_2.bmp");
    bmpButton_load_pic(page->fork_account_button);

    page->fork_password_button                 = request_bmpButton_direct();
    page->fork_password_button->center_cord.x  = 220;
    page->fork_password_button->center_cord.y  = 0;
    page->fork_password_button->handle_release = fork_password_action;
    bmpButton_set_release_pic_path(page->fork_password_button, "res/button/fork_1.bmp");
    bmpButton_set_press_pic_path(page->fork_password_button, "res/button/fork_2.bmp");
    bmpButton_load_pic(page->fork_password_button);



    for (int i = 0; i <= 9; i++) {
        char temp_path_str1[256];
        char temp_path_str2[256];
        memset(temp_path_str1, 0, sizeof(temp_path_str1));
        sprintf(temp_path_str1, "res/number/number_%d_1.bmp", i);

        memset(temp_path_str2, 0, sizeof(temp_path_str2));
        sprintf(temp_path_str2, "res/number/number_%d_2.bmp", i);

        page->number_button[i]                 = request_bmpButton_direct();
        page->number_button[i]->center_cord.x  = -270 + 60 * i;
        page->number_button[i]->center_cord.y  = 80;
        page->number_button[i]->handle_release = numberButton_action;
        bmpButton_set_release_pic_path(page->number_button[i], temp_path_str1);
        bmpButton_set_press_pic_path(page->number_button[i], temp_path_str2);
        bmpButton_load_pic(page->number_button[i]);
    }
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

    painter_draw_rectangle(-75, -100, 250, 50, White);
    painter_draw_rectangle(-75, -25, 250, 50, White);


    painter_draw_str(-170, -100, 50, "账号:");
    painter_draw_str(-170, -25, 50, "密码:");

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

            blankButton_analyze_touch(page->inputBox_account_button, get_touch_status_data());
            blankButton_analyze_touch(page->inputBox_password_button, get_touch_status_data());

            bmpButton_analyze_touch(page->login_button, get_touch_status_data());
            bmpButton_draw(page->login_button);
            bmpButton_analyze_touch(page->regist_button, get_touch_status_data());
            bmpButton_draw(page->regist_button);

            bmpButton_analyze_touch(page->fork_account_button, get_touch_status_data());
            bmpButton_draw(page->fork_account_button);
            bmpButton_analyze_touch(page->fork_password_button, get_touch_status_data());
            bmpButton_draw(page->fork_password_button);



            for (cur_loop_numb = 0; cur_loop_numb <= 9; cur_loop_numb++) {
                bmpButton_analyze_touch(page->number_button[cur_loop_numb],
                                        get_touch_status_data());
                bmpButton_draw(page->number_button[cur_loop_numb]);
            }
        }

        frameBuffer_display_frame();
    }

    while (*page_order_addr == LOGIN_PAGE);
}


void inputBox_account_action()
{
    cur_input_focus = 0;
}

void inputBox_password_action()
{
    cur_input_focus = 1;
}



void loginButton_action()
{
    *store_page_order_addr = NO_PAGE;
}

void fork_account_action()
{
    account_numbs = 0;
    painter_draw_rectangle(-75, -100, 250, 50, White);
}

void fork_password_action()
{
    password_numbs = 0;
    painter_draw_rectangle(-75, -25, 250, 50, White);
}


void numberButton_action()
{
    char draw_str[2];
    sprintf(draw_str, "%d", cur_loop_numb);

    switch (cur_input_focus) {
    case 0:
        if (account_numbs <= 10) {
            painter_draw_str(-70 + account_numbs * 20, -100, 50, draw_str);
            account_numbs += 1;
            // printf("1\n");
            break;
        }
    case 1:
        if (password_numbs <= 10) {
            painter_draw_str(-70 + password_numbs * 20, -25, 50, "*");
            password_numbs += 1;
            break;
        }
    }
}
