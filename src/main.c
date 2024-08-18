﻿#include "page/loginPage.h"
#include "page/desktopPage.h"

#include "decTypes.h"

#include "device/frameBuffer.h"
#include "device/touchScreen.h"
#include "painter.h"
#include "accountInfo.h"


#include <stdlib.h>
#include <time.h>

accountInfo_t* account_list_head;
struct tm      system_initiate_time;


int main(int argc, char** argv)
{
    account_list_head = request_account_node_direct();

    system_initiate_time.tm_year = 24;
    system_initiate_time.tm_mon  = 8 - 1;
    system_initiate_time.tm_mday = 18;

    if (argc == 4) {
        system_initiate_time.tm_hour = (argv[1][1] == '\0')
                                           ? (argv[1][0] - '0')
                                           : (argv[1][0] - '0') * 10 + (argv[1][1] - '0');
        system_initiate_time.tm_min  = (argv[2][1] == '\0')
                                           ? (argv[2][0] - '0')
                                           : (argv[2][0] - '0') * 10 + (argv[2][1] - '0');
        system_initiate_time.tm_sec  = (argv[3][1] == '\0')
                                           ? (argv[3][0] - '0')
                                           : (argv[3][0] - '0') * 10 + (argv[3][1] - '0');

        printf("%s %s %s\n", argv[1], argv[2], argv[3]);
        printf("%d %d %d\n",
               system_initiate_time.tm_hour,
               system_initiate_time.tm_min,
               system_initiate_time.tm_sec);
    }
    else {
        system_initiate_time.tm_hour = 10;
        system_initiate_time.tm_min  = 10;
        system_initiate_time.tm_sec  = 10;
    }

    struct loginPage*   login_page;
    struct desktopPage* desktop_page;

    enum PAGE cur_page = LOGIN_PAGE;

    init_touchScreen_device();
    init_frameBuffer_device();
    init_painter();

    desktop_page = request_desktopPage_direct();
    login_page   = request_loginPage_direct();



    while (cur_page != NO_PAGE) {
        switch (cur_page) {
        case LOGIN_PAGE:
            init_loginPage(login_page);
            run_loginPage(login_page, &cur_page);
            break;
        case DESKTOP_PAGE:
            init_desktopPage(desktop_page);
            run_desktopPage(desktop_page, &cur_page);
            break;
        default: cur_page = NO_PAGE; break;
        }
    }

    // destroy_loginPage(login_page);
    // destroy_desktopPage(desktop_page);
}