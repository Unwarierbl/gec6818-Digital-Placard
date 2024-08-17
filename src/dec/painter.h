#ifndef PAINTER_H
#define PAINTER_H

void init_painter();      // ok
void destroy_painter();   // ok

void painter_draw_RGB_point(int x, int y, unsigned int color);    // ok
void painter_draw_ARGB_point(int x, int y, unsigned int color);   // ok

// void painter_draw_font(int x, int y, int font_size, char* font_str);
// void painter_clear_font(int x, int y, int font_size, char* font_str);

void painter_clear_range(int x, int y, int width, int height);

void painter_draw_circle(int x_center, int y_center, int radius, unsigned int color);         // ok
void painter_draw_solid_circle(int x_center, int y_center, int radius, unsigned int color);   // ok

void painter_draw_rectangle(int x, int y, int width, int height, unsigned int color);   // ok

void painter_draw_RGB_pic(unsigned int* pic_buffer, int x, int y, int width, int height);   // ok
void painter_draw_RGB_pic_zoom(unsigned int* pic_buffer, int x, int y, int width,
                               int   height,   // ok
                               float scale);
void painter_draw_RGB_pic_zoom_out(unsigned int* pic_buffer, int x, int y, int width,
                                   int   height,   // ok
                                   float scale);
void painter_draw_RGB_pic_zoom_in(unsigned int* pic_buffer, int x, int y, int width,
                                  int   height,   // ok
                                  float scale);

// 此函数用来处理透明度
void painter_draw_ARGB_pic(unsigned int* pic_buffer, int x, int y, int width, int height);   // ok
void painter_draw_ARGB_pic_zoom(unsigned int* pic_buffer, int x, int y, int width,
                                int   height,   // ok
                                float scale);
void painter_draw_ARGB_pic_zoom_out(unsigned int* pic_buffer, int x, int y, int width,
                                    int   height,   // ok
                                    float scale);
void painter_draw_ARGB_pic_zoom_in(unsigned int* pic_buffer, int x, int y, int width,
                                   int   height,   // ok
                                   float scale);

#endif