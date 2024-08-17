#include "device/frameBuffer.h"

#include "decTypes.h"

#include <fcntl.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

int screen_edge_up;
int screen_edge_down;
int screen_edge_left;
int screen_edge_right;


static int           fb_fd;     // 帧缓冲设备文件描述符
static unsigned int* fb_addr;   // 显存映射到虚拟内存的地址

static unsigned int* fb_buffer;      // 数据缓冲区
static unsigned int* fb_bg_buffer;   // 屏幕背景



int init_frameBuffer_device()
{
    fb_fd = open("/dev/fb0", O_RDWR);
    if (fb_fd == -1) {
        perror("打开显存文件失败");
        return -1;
    }

    fb_addr = (unsigned int*)mmap(NULL,   // 获取显存映射到虚拟内存的地址
                                  800 * 480 * 4,
                                  PROT_READ | PROT_WRITE,
                                  MAP_SHARED,
                                  fb_fd,
                                  0);
    if (fb_addr == MAP_FAILED) {
        perror("映射显存内存失败");
        close(fb_fd);
        return -1;
    }

    fb_buffer    = (unsigned int*)malloc(480 * 800 * 4);
    fb_bg_buffer = (unsigned int*)malloc(480 * 800 * 4);
    // fb_bg_temp_buffer = (unsigned int*)malloc(480 * 800 * 4);

    if (fb_buffer == NULL || fb_bg_buffer == NULL) {
        return -1;
    }

    screen_edge_left  = -400;
    screen_edge_right = 400 - 1;
    screen_edge_up    = -240;
    screen_edge_down  = 240 - 1;

    frameBuffer_set_bg_as_color((unsigned int)Black);

    clear_screen();
    frameBuffer_display_frame();

    return 0;
}


void destroy_frameBuffer_device()
{
    munmap(fb_addr, 800 * 480 * 4);

    free(fb_buffer);
    free(fb_bg_buffer);

    close(fb_fd);
}

unsigned int* get_frameBuffer_cur_buffer()
{
    return fb_buffer;
}



void frameBuffer_display_frame()
{
    memcpy(fb_addr, fb_buffer, 800 * 480 * 4);
}

void frameBuffer_set_bg_as_color(unsigned int color)
{
    for (int y = 0; y < 480; ++y) {
        for (int x = 0; x < 800; ++x) {
            *(fb_bg_buffer + 800 * y + x) = color;
        }
    }
}


void frameBuffer_set_bg_as_cur_display()
{
    memcpy(fb_bg_buffer, fb_buffer, 800 * 480 * 4);
}

void clear_screen()
{
    memcpy(fb_buffer, fb_bg_buffer, 480 * 800 * 4);
}

void clear_screen_range(int x, int y, int width, int height)
{
    // printf("clean x:%d,y:%d ", x, y);
    // printf("clean width:%d,height:%d\n", width, height);
    for (int row = y + 240; row < y + 240 + height; row++) {
        if (row < 0 || row > 479) {
            continue;
        }
        if (x + 400 + width < 800) {
            memcpy(fb_buffer + 800 * row + x + 400,
                   fb_bg_buffer + 800 * row + x + 400,
                   width * sizeof(unsigned int));
        }
        else {
            memcpy(fb_buffer + 800 * row + x + 400,
                   fb_bg_buffer + 800 * row + x + 400,
                   (799 - x - 400) * sizeof(unsigned int));
        }
    }
}

void set_point_RGB_color(int x, int y, unsigned int color)
{
    if (x < screen_edge_left || x > screen_edge_right || y < screen_edge_up ||
        y > screen_edge_down) {
        return;
    }

    *(fb_buffer + (y + 240) * 800 + (x + 400)) = color;
}

void set_point_ARGB_color(int x, int y, unsigned int color)
{
    float transp = color >> 8 * 3;
    transp       = transp / 255.0f;


    unsigned int cur_color = *(fb_buffer + (y + 240) * 800 + (x + 400));
    unsigned int new_color;

    Pixel cur_pixel = {.Red   = (cur_color >> 8 * 2) & 0xff,
                       .Green = (cur_color >> 8 * 1) & 0xff,
                       .Blue  = (cur_color >> 8 * 0) & 0xff};

    Pixel new_pixel = {.Red   = (color >> 8 * 2) & 0xff,   //
                       .Green = (color >> 8 * 1) & 0xff,
                       .Blue  = (color >> 8 * 0) & 0xff};

    new_pixel.Red   = transp * new_pixel.Red + (1 - transp) * cur_pixel.Red;
    new_pixel.Green = transp * new_pixel.Green + (1 - transp) * cur_pixel.Green;
    new_pixel.Blue  = transp * new_pixel.Blue + (1 - transp) * cur_pixel.Blue;

    new_color = new_pixel.Red << 8 * 2 |     //
                new_pixel.Green << 8 * 1 |   //
                new_pixel.Blue << 8 * 0;


    set_point_RGB_color(x, y, new_color);
}