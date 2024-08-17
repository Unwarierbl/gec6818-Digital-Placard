#ifndef DEC_TYPES_H
#define DEC_TYPES_H

#include "list.h"
#include <stdio.h>
#include <stdbool.h>

#define FRAME_DELTA_TIME (24)   // 单位 ms


#ifndef true
#    define true (1)
#endif

#ifndef false
#    define false (0)
#endif

typedef struct
{
    int x, y;
} vec2;
typedef struct
{
    float x, y;
} vec2f;


typedef struct ARGB_color
{
    char Alpha;
    char Red;
    char Green;
    char Blue;
} Pixel;

enum Color
{
    Red      = 0xffff0000u,   // 红
    Green    = 0xff00ff00u,   // 绿
    Blue     = 0xff0000ffu,   // 蓝
    Cyan     = 0xff00FFFFu,   // 青
    White    = 0xffffffffu,   // 白
    DeepPink = 0xffFF1493u,   // 深粉
    Yellow   = 0xffFFFF00u,   // 黄
    Gray     = 0xff808080u,   // 灰
    Black    = 0xff000000u    // 黑
};

enum PAGE
{

    NO_PAGE,
    LOGIN_PAGE,

};

enum TOUCH_STATUS
{
    NO_TOUCH,
    PRESS,
    HOLD,
    SLIDE,
    RELEASE
};

enum TOUCH_SLIDE_STATUS
{
    NO_SLIDE         = 0x00,
    SLIDE_LEFT       = 0x01,
    SLIDE_RIGHT      = 0x02,
    SLIDE_DOWN       = 0x04,
    SLIDE_UP         = 0x08,
    SLIDE_LEFT_UP    = SLIDE_LEFT | SLIDE_UP,
    SLIDE_LEFT_DOWN  = SLIDE_LEFT | SLIDE_DOWN,
    SLIDE_RIGHT_UP   = SLIDE_RIGHT | SLIDE_UP,
    SLIDE_RIGHT_DOWN = SLIDE_RIGHT | SLIDE_DOWN,
};

typedef struct
{
    enum TOUCH_STATUS touch_status;

    vec2 cur_touch_cord;
    vec2 pre_touch_cord;
    vec2 press_cord;
    vec2 release_cord;

    enum TOUCH_SLIDE_STATUS slide_status;
    vec2                    slide_offset;
} touchStatusData;



#pragma pack(1)
// bmp 图像
typedef struct
{
    unsigned short
        bfType;   // 19778，必须是BM字符串，对应的十六进制为0x4d42,十进制为19778，否则不是bmp格式文件
    unsigned int   bfSize;        // 文件大小 以字节为单位(2-5字节)
    unsigned short bfReserved1;   // 保留，必须设置为0 (6-7字节)
    unsigned short bfReserved2;   // 保留，必须设置为0 (8-9字节)
    unsigned int   bfOffBits;     // 从文件头到像素数据的偏移  (10-13字节)

} bitmapFileHeader;

typedef struct
{
    unsigned int biSize;     // 此结构体的大小 (14-17字节)
    long         biWidth;    // 图像的宽  (18-21字节)
    long         biHeight;   // 图像的高  (22-25字节)
    unsigned short
        biPlanes;   // 表示bmp图片的平面属，显然显示器只有一个平面，所以恒等于1 (26-27字节)
    unsigned short biBitCount;   // 一像素所占的位数，一般为24   (28-29字节)
    unsigned int biCompression;   // 说明图象数据压缩的类型，0为不压缩。 (30-33字节)
    unsigned int biSizeImage;   // 像素数据所占大小, 这个值应该等于上面文件头结构中bfSize-bfOffBits
                                // (34-37字节)
    long biXPelsPerMeter;   // 说明水平分辨率，用象素/米表示。一般为0 (38-41字节)
    long biYPelsPerMeter;   // 说明垂直分辨率，用象素/米表示。一般为0 (42-45字节)
    unsigned int
        biClrUsed;   // 说明位图实际使用的彩色表中的颜色索引数（设为0的话，则说明使用所有调色板项）。
                     // (46-49字节)
    unsigned int
        biClrImportant;   // 说明对图象显示有重要影响的颜色索引的数目，如果是0，表示都重要。(50-53字节)

} bitmapInfoHeader;
#pragma pack()

#endif