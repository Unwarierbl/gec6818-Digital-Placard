#ifndef LOGIN_PAGE_H
#define LOGIN_PAGE_H


struct jpegPicture;
struct bmpButton;
struct blankButton;

enum PAGE;

struct loginPage
{
    struct jpegPicture* bg_jpeg_picture;   // ok

    struct blankButton* inputBox_account_button;    // ok
    struct blankButton* inputBox_password_button;   // ok

    struct bmpButton* login_button;    // ok
    struct bmpButton* regist_button;   // ok

    struct bmpButton* fork_account_button;    // ok
    struct bmpButton* fork_password_button;   // ok

    struct bmpButton* number_button[10];   // ok
};

struct loginPage* request_loginPage_direct();   // ok

void init_loginPage(struct loginPage* page);      // ok
void destroy_loginPage(struct loginPage* page);   // ok

void run_loginPage(struct loginPage* page, enum PAGE* page_order);   // ok

void inputBox_account_action();    // ok
void inputBox_password_action();   // ok

void loginButton_action();    // ok
void registButton_action();   // ok

void fork_account_action();   // ok
void fork_password_action();

void numberButton_action();   // ok

#endif