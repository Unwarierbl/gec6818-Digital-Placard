#include "painter.h"
#include "device/frameBuffer.h"


#include <math.h>
#include <decTypes.h>
#include <font.h>

static unsigned int* store_frameBuffer_addr;

static void (*basic_draw_RGB_point)(int x, int y, unsigned int color);
static void (*basic_draw_ARGB_point)(int x, int y, unsigned int color);

static font*  f;
static bitmap bm;


void init_painter()
{
    store_frameBuffer_addr = get_frameBuffer_cur_buffer();

    basic_draw_RGB_point  = set_point_RGB_color;
    basic_draw_ARGB_point = set_point_ARGB_color;


    bm.height       = 480;
    bm.width        = 800;
    bm.byteperpixel = 4;
    bm.map          = (void*)store_frameBuffer_addr;   // 指定画板是显存地址



    // fontPrint(
    //     f,
    //     &bm,
    //     100,
    //     100,
    //     "靓仔",
    //     getColor(0, 0, 0xff, 0),
    //     0);   // fontPrint(字体资源， 显存资源， x坐标，y坐标，显示的字符串, 字体颜色, 默认写0);
}
void destroy_painter()
{}

void painter_draw_RGB_point(int x, int y, unsigned int color)
{
    basic_draw_RGB_point(x, y, color);
}
void painter_draw_ARGB_point(int x, int y, unsigned int color)
{
    basic_draw_ARGB_point(x, y, color);
}

void painter_draw_str(int x, int y, int font_size, char* draw_str)
{
    f = fontLoad("/usr/share/fonts/SourceHanSerifCN-Heavy.ttf");
    fontSetSize(f, 50);

    fontPrint(
        f,
        &bm,
        x + 400,
        y + 240,
        draw_str,
        getColor(0, 0, 0x00, 0),
        0);   // fontPrint(字体资源， 显存资源， x坐标，y坐标，显示的字符串, 字体颜色, 默认写0);

    fontUnload(f);
}


void painter_clear_range(int x, int y, int width, int height)
{
    clear_screen_range(x, y, width, height);
}



void painter_draw_circle(int x_center, int y_center, int radius, unsigned int color)
{
    int d0, x_cur, y_cur;
    x_cur = 0;
    y_cur = radius;
    d0    = 3 - 2 * radius;

    while (x_cur < y_cur) {
        if (d0 >= 0) {
            d0 = d0 + 4 * (x_cur - y_cur) + 10;
            x_cur += 1;
            y_cur -= 1;

            painter_draw_RGB_point(x_center + x_cur, y_center + y_cur, color);
            painter_draw_RGB_point(x_center - x_cur, y_center + y_cur, color);
            painter_draw_RGB_point(x_center - x_cur, y_center - y_cur, color);
            painter_draw_RGB_point(x_center + x_cur, y_center - y_cur, color);

            painter_draw_RGB_point(x_center + y_cur, y_center + x_cur, color);
            painter_draw_RGB_point(x_center - y_cur, y_center + x_cur, color);
            painter_draw_RGB_point(x_center - y_cur, y_center - x_cur, color);
            painter_draw_RGB_point(x_center + y_cur, y_center - x_cur, color);
        }

        else {
            d0 = d0 + 4 * x_cur + 6;
            x_cur += 1;
            y_cur = y_cur;

            painter_draw_RGB_point(x_center + x_cur, y_center + y_cur, color);
            painter_draw_RGB_point(x_center - x_cur, y_center + y_cur, color);
            painter_draw_RGB_point(x_center - x_cur, y_center - y_cur, color);
            painter_draw_RGB_point(x_center + x_cur, y_center - y_cur, color);

            painter_draw_RGB_point(x_center + y_cur, y_center + x_cur, color);
            painter_draw_RGB_point(x_center - y_cur, y_center + x_cur, color);
            painter_draw_RGB_point(x_center - y_cur, y_center - x_cur, color);
            painter_draw_RGB_point(x_center + y_cur, y_center - x_cur, color);
        }
    }
}

void painter_draw_solid_circle(int x_center, int y_center, int radius, unsigned int color)
{
    int tx, ty, d, i;
    tx = 0;
    ty = radius;
    d  = 3 - 2 * radius;

    while (tx < ty) {
        for (i = x_center - ty; i <= x_center + ty; i++) {
            painter_draw_RGB_point(i, y_center - tx, color);

            if (tx != 0) {
                painter_draw_RGB_point(i, y_center + tx, color);
            }
        }

        if (d < 0) {
            d += 4 * tx + 6;
        }
        else {
            for (i = x_center - tx; i <= x_center + tx; i++) {
                painter_draw_RGB_point(i, y_center - ty, color);
                painter_draw_RGB_point(i, y_center + ty, color);
            }

            d += 4 * (tx - ty) + 10;
            ty -= 1;
        }
        tx += 1;
    }

    if (tx == ty) {
        for (i = x_center - ty; i <= x_center + ty; i++) {
            painter_draw_RGB_point(i, y_center - tx, color);
            painter_draw_RGB_point(i, y_center + tx, color);
        }
    }
}


void painter_draw_rectangle(int x, int y, int width, int height, unsigned int color)
{
    int tx, ty;

    for (ty = y; ty < y + height; ++ty) {
        for (tx = x; tx < x + width; ++tx) {
            painter_draw_RGB_point(tx, ty, color);
        }
    }
}



void painter_draw_RGB_pic(unsigned int* pic_buffer, int x, int y, int width, int height)
{
    if (pic_buffer == NULL) {
        return;
    }

    int tx, ty;
    for (ty = y; ty < y + height; ++ty) {
        for (tx = x; tx < x + width; ++tx) {

            painter_draw_RGB_point(tx, ty, *(pic_buffer + width * (ty + 240) + (tx + 400)));
        }
    }
}


void painter_draw_RGB_pic_zoom(unsigned int* pic_buffer, int x, int y, int width, int height,
                               float scale)
{
    // printf("scale=%f,x=%d,y=%d,width=%d,height=%d\n", scale, x, y, width, height);

    if (scale == 1.0f) {
        painter_draw_RGB_pic(pic_buffer, x, y, width, height);
    }
    else if (scale < 1.0f) {
        painter_draw_RGB_pic_zoom_in(pic_buffer,
                                     x + width * 0.5f * (1 - scale),
                                     y + height * 0.5f * (1 - scale),
                                     width,
                                     height,
                                     scale);
    }
    else if (scale > 1.0f) {
        painter_draw_RGB_pic_zoom_out(pic_buffer,
                                      x + width * 0.5f * (1 - scale),
                                      y + height * 0.5f * (1 - scale),
                                      width,
                                      height,
                                      scale);
        // painter_draw_RGB_pic_zoom_out(pic_buffer, x, y, width, height, scale);
    }
}

void painter_draw_RGB_pic_zoom_out(unsigned int* pic_buffer, int x, int y, int width, int height,
                                   float scale)
{
    if (pic_buffer == NULL) {
        return;
    }

    unsigned int trans_color;

    unsigned int      temp_color;
    struct ARGB_color temp_color_data;
    struct ARGB_color temp_color_data_1;
    struct ARGB_color temp_color_data_2;
    struct ARGB_color temp_color_data_3;
    struct ARGB_color temp_color_data_4;

    float interval = 1.0f / scale;

    int   final_x, final_y;
    float sample_x, sample_y;

    for (sample_y = 0, final_y = 0; sample_y < height; sample_y += interval, ++final_y) {
        for (sample_x = 0, final_x = 0; sample_x < width; sample_x += interval, ++final_x) {

            double x2 = sample_x;
            double y2 = sample_y;
            int    x1 = (int)x2;
            int    y1 = (int)y2;

            double s1 = y2 - y1;
            double s4 = x2 - x1;
            double s2 = 1 - s4;
            double s3 = 1 - s1;

            if (x1 >= width - 1) x1 = width - 2;
            if (y1 >= height - 1) y1 = height - 2;

            temp_color              = *(pic_buffer + width * y1 + x1);
            temp_color_data_1.Red   = (temp_color >> 8 * 2) & 0x000000ff;
            temp_color_data_1.Green = (temp_color >> 8 * 1) & 0x000000ff;
            temp_color_data_1.Blue  = (temp_color >> 8 * 0) & 0x000000ff;

            temp_color              = *(pic_buffer + width * y1 + x1);
            temp_color_data_2.Red   = (temp_color >> 8 * 2) & 0x000000ff;
            temp_color_data_2.Green = (temp_color >> 8 * 1) & 0x000000ff;
            temp_color_data_2.Blue  = (temp_color >> 8 * 0) & 0x000000ff;

            temp_color              = *(pic_buffer + width * y1 + x1);
            temp_color_data_3.Red   = (temp_color >> 8 * 2) & 0x000000ff;
            temp_color_data_3.Green = (temp_color >> 8 * 1) & 0x000000ff;
            temp_color_data_3.Blue  = (temp_color >> 8 * 0) & 0x000000ff;

            temp_color              = *(pic_buffer + width * y1 + x1);
            temp_color_data_4.Red   = (temp_color >> 8 * 2) & 0x000000ff;
            temp_color_data_4.Green = (temp_color >> 8 * 1) & 0x000000ff;
            temp_color_data_4.Blue  = (temp_color >> 8 * 0) & 0x000000ff;

            temp_color_data.Red = temp_color_data_1.Red * s1 * s4 +
                                  temp_color_data_2.Red * s1 * s2 +
                                  temp_color_data_3.Red * s2 * s3 + temp_color_data_4.Red * s3 * s4;
            temp_color_data.Green =
                temp_color_data_1.Green * s1 * s4 + temp_color_data_2.Green * s1 * s2 +
                temp_color_data_3.Green * s2 * s3 + temp_color_data_4.Green * s3 * s4;
            temp_color_data.Blue =
                temp_color_data_1.Blue * s1 * s4 + temp_color_data_2.Blue * s1 * s2 +
                temp_color_data_3.Blue * s2 * s3 + temp_color_data_4.Blue * s3 * s4;

            trans_color = temp_color_data.Red << (8 * 2) | temp_color_data.Green << (8 * 1) |
                          temp_color_data.Blue << (8 * 0);

            painter_draw_RGB_point(final_x + x, final_y + y, trans_color);
        }
    }
}

void painter_draw_RGB_pic_zoom_in(unsigned int* pic_buffer, int x, int y, int width, int height,
                                  float scale)
{
    if (pic_buffer == NULL) {
        return;
    }

    unsigned int      trans_color;
    unsigned int      temp_color;
    struct ARGB_color temp_color_data;

    float interval = 1.0f / scale;

    int   final_x, final_y;
    float sample_x, sample_y;
    int   interpol_x, interpol_y;

    for (sample_y = 0, final_y = 0; sample_y < height; sample_y += interval, ++final_y) {
        for (sample_x = 0, final_x = 0; sample_x < width; sample_x += interval, ++final_x) {

            temp_color = *(pic_buffer + width * (int)sample_y + (int)sample_x);

            temp_color_data.Red   = (temp_color >> 8 * 2) & 0x000000ff;
            temp_color_data.Green = (temp_color >> 8 * 1) & 0x000000ff;
            temp_color_data.Blue  = (temp_color >> 8 * 0) & 0x000000ff;

            for (interpol_y = sample_y + 1; interpol_y < sample_y + interval; ++interpol_y) {
                for (interpol_x = sample_x + 1; interpol_x < sample_x + interval; ++interpol_x) {

                    temp_color = *(pic_buffer + width * interpol_y + interpol_x);
                    temp_color_data.Red =
                        temp_color_data.Red / 2 + ((temp_color >> 8 * 2) & 0x000000ff) / 2;
                    temp_color_data.Green =
                        temp_color_data.Green / 2 + ((temp_color >> 8 * 1) & 0x000000ff) / 2;
                    temp_color_data.Blue =
                        temp_color_data.Blue / 2 + ((temp_color >> 8 * 0) & 0x000000ff) / 2;
                }
            }
            trans_color = temp_color_data.Red << (8 * 2) | temp_color_data.Green << (8 * 1) |
                          temp_color_data.Blue << (8 * 0);

            painter_draw_RGB_point(final_x + x, final_y + y, trans_color);
        }
    }
}



void painter_draw_ARGB_pic(unsigned int* pic_buffer, int x, int y, int width, int height)
{
    if (pic_buffer == NULL) {
        return;
    }

    int tx, ty;
    for (ty = 0; ty < height; ++ty) {
        for (tx = 0; tx < width; ++tx) {

            // printf("point x=%d,y=%d\n", (tx), (ty));
            painter_draw_ARGB_point(x + tx, y + ty, *(pic_buffer + width * ty + tx));
        }
    }
}


void painter_draw_ARGB_pic_zoom(unsigned int* pic_buffer, int x, int y, int width,   // ok
                                int height, float scale)
{
    if (scale == 1.0f) {
        painter_draw_ARGB_pic(pic_buffer, x, y, width, height);
    }
    else if (scale < 1.0f) {
        painter_draw_ARGB_pic_zoom_in(pic_buffer,
                                      x + width * 0.5f * (1 - scale),
                                      y + height * 0.5f * (1 - scale),
                                      width,
                                      height,
                                      scale);
    }
    else if (scale > 1.0f) {
        painter_draw_ARGB_pic_zoom_out(pic_buffer,
                                       x + width * 0.5f * (1 - scale),
                                       y + height * 0.5f * (1 - scale),
                                       width,
                                       height,
                                       scale);
    }
}

void painter_draw_ARGB_pic_zoom_out(unsigned int* pic_buffer, int x, int y, int width,   // ok
                                    int height, float scale)
{
    if (pic_buffer == NULL) {
        return;
    }

    unsigned int trans_color;

    unsigned int      temp_color;
    struct ARGB_color temp_color_data;
    struct ARGB_color temp_color_data_1;
    struct ARGB_color temp_color_data_2;
    struct ARGB_color temp_color_data_3;
    struct ARGB_color temp_color_data_4;

    float interval = 1.0f / scale;

    int   final_x, final_y;
    float sample_x, sample_y;

    for (sample_y = 0, final_y = 0; sample_y < height; sample_y += interval, ++final_y) {
        for (sample_x = 0, final_x = 0; sample_x < width; sample_x += interval, ++final_x) {

            double x2 = sample_x;
            double y2 = sample_y;
            int    x1 = (int)x2;
            int    y1 = (int)y2;

            double s1 = y2 - y1;
            double s4 = x2 - x1;
            double s2 = 1 - s4;
            double s3 = 1 - s1;

            if (x1 >= width - 1) x1 = width - 2;
            if (y1 >= height - 1) y1 = height - 2;

            temp_color              = *(pic_buffer + width * y1 + x1);
            temp_color_data_1.Alpha = (temp_color >> 8 * 3) & 0x000000ff;
            temp_color_data_1.Red   = (temp_color >> 8 * 2) & 0x000000ff;
            temp_color_data_1.Green = (temp_color >> 8 * 1) & 0x000000ff;
            temp_color_data_1.Blue  = (temp_color >> 8 * 0) & 0x000000ff;

            temp_color              = *(pic_buffer + width * y1 + x1);
            temp_color_data_2.Alpha = (temp_color >> 8 * 1) & 0x000000ff;
            temp_color_data_2.Red   = (temp_color >> 8 * 2) & 0x000000ff;
            temp_color_data_2.Green = (temp_color >> 8 * 1) & 0x000000ff;
            temp_color_data_2.Blue  = (temp_color >> 8 * 0) & 0x000000ff;

            temp_color              = *(pic_buffer + width * y1 + x1);
            temp_color_data_3.Alpha = (temp_color >> 8 * 3) & 0x000000ff;
            temp_color_data_3.Red   = (temp_color >> 8 * 2) & 0x000000ff;
            temp_color_data_3.Green = (temp_color >> 8 * 1) & 0x000000ff;
            temp_color_data_3.Blue  = (temp_color >> 8 * 0) & 0x000000ff;

            temp_color              = *(pic_buffer + width * y1 + x1);
            temp_color_data_4.Alpha = (temp_color >> 8 * 3) & 0x000000ff;
            temp_color_data_4.Red   = (temp_color >> 8 * 2) & 0x000000ff;
            temp_color_data_4.Green = (temp_color >> 8 * 1) & 0x000000ff;
            temp_color_data_4.Blue  = (temp_color >> 8 * 0) & 0x000000ff;


            temp_color_data.Alpha =
                temp_color_data_1.Alpha * s1 * s4 + temp_color_data_2.Alpha * s1 * s2 +
                temp_color_data_3.Alpha * s2 * s3 + temp_color_data_4.Alpha * s3 * s4;
            temp_color_data.Red = temp_color_data_1.Red * s1 * s4 +
                                  temp_color_data_2.Red * s1 * s2 +
                                  temp_color_data_3.Red * s2 * s3 + temp_color_data_4.Red * s3 * s4;
            temp_color_data.Green =
                temp_color_data_1.Green * s1 * s4 + temp_color_data_2.Green * s1 * s2 +
                temp_color_data_3.Green * s2 * s3 + temp_color_data_4.Green * s3 * s4;
            temp_color_data.Blue =
                temp_color_data_1.Blue * s1 * s4 + temp_color_data_2.Blue * s1 * s2 +
                temp_color_data_3.Blue * s2 * s3 + temp_color_data_4.Blue * s3 * s4;

            trans_color = temp_color_data.Alpha << (8 * 3) | temp_color_data.Red << (8 * 2) |
                          temp_color_data.Green << (8 * 1) | temp_color_data.Blue << (8 * 0);

            painter_draw_ARGB_point(final_x + x, final_y + y, trans_color);
        }
    }
}

void painter_draw_ARGB_pic_zoom_in(unsigned int* pic_buffer, int x, int y, int width,   // ok
                                   int height, float scale)
{
    if (pic_buffer == NULL) {
        return;
    }

    unsigned int      trans_color;
    unsigned int      temp_color;
    struct ARGB_color temp_color_data;

    float interval = 1.0f / scale;

    int   final_x, final_y;
    float sample_x, sample_y;
    int   interpol_x, interpol_y;

    for (sample_y = 0, final_y = 0; sample_y < height; sample_y += interval, ++final_y) {
        for (sample_x = 0, final_x = 0; sample_x < width; sample_x += interval, ++final_x) {

            temp_color = *(pic_buffer + width * (int)sample_y + (int)sample_x);

            temp_color_data.Alpha = (temp_color >> 8 * 3) & 0x000000ff;
            temp_color_data.Red   = (temp_color >> 8 * 2) & 0x000000ff;
            temp_color_data.Green = (temp_color >> 8 * 1) & 0x000000ff;
            temp_color_data.Blue  = (temp_color >> 8 * 0) & 0x000000ff;

            for (interpol_y = sample_y + 1; interpol_y < sample_y + interval; ++interpol_y) {
                for (interpol_x = sample_x + 1; interpol_x < sample_x + interval; ++interpol_x) {

                    temp_color = *(pic_buffer + width * interpol_y + interpol_x);
                    temp_color_data.Alpha =
                        temp_color_data.Alpha / 2 + ((temp_color >> 8 * 3) & 0x000000ff) / 2;
                    temp_color_data.Red =
                        temp_color_data.Red / 2 + ((temp_color >> 8 * 2) & 0x000000ff) / 2;
                    temp_color_data.Green =
                        temp_color_data.Green / 2 + ((temp_color >> 8 * 1) & 0x000000ff) / 2;
                    temp_color_data.Blue =
                        temp_color_data.Blue / 2 + ((temp_color >> 8 * 0) & 0x000000ff) / 2;
                }
            }
            trans_color = temp_color_data.Alpha << (8 * 3) | temp_color_data.Red << (8 * 2) |
                          temp_color_data.Green << (8 * 1) | temp_color_data.Blue << (8 * 0);

            painter_draw_ARGB_point(final_x + x, final_y + y, trans_color);
        }
    }
}