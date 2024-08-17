#ifndef LOGIN_PAGE_H
#define LOGIN_PAGE_H


struct jpegPicture;
struct bmpButton;

enum PAGE;

struct loginPage
{
    struct jpegPicture* bg_jpeg_picture;

    struct bmpButton* login_button;
};

struct loginPage* request_loginPage_direct();   // ok

void init_loginPage(struct loginPage* page);      // ok
void destroy_loginPage(struct loginPage* page);   // ok

void run_loginPage(struct loginPage* page, enum PAGE* page_order);   // ok

void loginButton_action();

#endif