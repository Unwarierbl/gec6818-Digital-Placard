#include "page/loginPage.h"
#include "decTypes.h"

#include "device/frameBuffer.h"
#include "device/touchScreen.h"
#include "painter.h"
#include "accountInfo.h"


#include <stdlib.h>
accountInfo_t* account_list_head;

int main(void)
{
    account_list_head = request_account_node_direct();

    struct loginPage* login_page;

    enum PAGE cur_page = LOGIN_PAGE;

    init_touchScreen_device();
    init_frameBuffer_device();
    init_painter();

    login_page = request_loginPage_direct();


    while (cur_page != NO_PAGE) {
        switch (cur_page) {
        case LOGIN_PAGE:
            init_loginPage(login_page);
            run_loginPage(login_page, &cur_page);
            break;
        default: cur_page = NO_PAGE; break;
        }
    }

    destroy_loginPage(login_page);
}