#include "module/bmpTimeDisplayer.h"

#include "painter.h"

#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include "decTypes.h"


extern struct tm system_initiate_time;

struct bmpTimeDisplayer* request_bmpTimeDisplayer_direct()
{
    struct bmpTimeDisplayer* new_button =
        (struct bmpTimeDisplayer*)malloc(sizeof(struct bmpTimeDisplayer));
    init_bmpTimeDisplayer(new_button);

    return new_button;
}


void init_bmpTimeDisplayer(struct bmpTimeDisplayer* displayer)
{
    // printf("timer init run\n");

    displayer->center_cord.x = 0;
    displayer->center_cord.y = 0;

    displayer->need_redraw = true;


    displayer->initiate_time = system_initiate_time;
    displayer->cur_real_time = system_initiate_time;

    printf("year=%d,mount=%d,day=%d\n",
           displayer->initiate_time.tm_year,
           displayer->initiate_time.tm_mon + 1,
           displayer->initiate_time.tm_mday);

    displayer->start_time = time(NULL);
    displayer->cur_time   = time(NULL);

    displayer->cur_tick_time = difftime(displayer->cur_time, displayer->start_time);
    displayer->pre_tick_time = displayer->cur_tick_time;


    for (int i = 0; i <= 6; i++) {
        memset(displayer->font_pic_path[i], 0, sizeof(displayer->font_pic_path[i]));
        sprintf(displayer->font_pic_path[i], "res/time_font/time_font_%d.bmp", i);
    }

    for (int i = 0; i <= 9; i++) {
        memset(displayer->number_pic_path[i], 0, sizeof(displayer->number_pic_path[i]));
        sprintf(displayer->number_pic_path[i], "res/number_2/number_%d.bmp", i);
    }

    bmpTimeDisplayer_load_pic(displayer);
}

void remove_bmpTimeDisplayer_direct(struct bmpTimeDisplayer* remove_displayer)
{
    for (int i = 0; i <= 6; i++) {
        if (remove_displayer->font_pic_buffer[i] != NULL) {
            free(remove_displayer->font_pic_buffer[i]);
        }
    }

    for (int i = 0; i <= 9; i++) {
        if (remove_displayer->number_pic_buffer[i] != NULL) {
            free(remove_displayer->number_pic_buffer[i]);
        }
    }

    free(remove_displayer);
}

void bmpTimeDisplayer_load_pic(struct bmpTimeDisplayer* displayer)
{
    for (int i = 0; i <= 5; i++) {

        if (strlen(displayer->font_pic_path[i]) != 0) {
            FILE* fp;
            if ((fp = fopen(displayer->font_pic_path[i], "rb")) == NULL) {
                fprintf(stderr, "can not open file :%s", displayer->font_pic_path[i]);
            }
            else {
                bitmapFileHeader bfHeader;
                fread(&bfHeader, 14, 1, fp);
                bitmapInfoHeader biHeader;
                fread(&biHeader, 40, 1, fp);

                int imSize   = biHeader.biSizeImage;
                int width    = biHeader.biWidth;
                int height   = biHeader.biHeight;
                int bitCount = biHeader.biBitCount;

                displayer->font_pic_length[i].x = width;
                displayer->font_pic_length[i].y = height;

                if (displayer->font_pic_max_length.x < width) {
                    displayer->font_pic_max_length.x = width;
                }
                if (displayer->font_pic_max_length.y < height) {
                    displayer->font_pic_max_length.y = height;
                }

                // 因为是32bit所以无所谓
                int line_width = (width + 3) / 4 * 4;
                int line_bytes = line_width * bitCount / 8;

                unsigned int* pic_buffer = malloc(width * height * sizeof(unsigned int));


                // 这里bfReserved2指向了offsetBits数据
                fseek(fp, bfHeader.bfOffBits, SEEK_SET);
                unsigned char* imageData = (unsigned char*)malloc(imSize * sizeof(unsigned char));
                memset(imageData, 0, imSize * sizeof(unsigned char));
                fread(imageData, imSize * sizeof(unsigned char), 1, fp);


                int          row, col;
                Pixel        temp_pixel;
                unsigned int temp_color_data;

                for (row = 0; row < height; row++) {
                    for (col = 0; col < width * 4; col += 4) {
                        temp_pixel.Blue  = *(imageData + line_bytes * (height - 1 - row) + col + 0);
                        temp_pixel.Green = *(imageData + line_bytes * (height - 1 - row) + col + 1);
                        temp_pixel.Red   = *(imageData + line_bytes * (height - 1 - row) + col + 2);
                        temp_pixel.Alpha = *(imageData + line_bytes * (height - 1 - row) + col + 3);


                        temp_color_data = temp_pixel.Alpha << 8 * 3 | temp_pixel.Red << 8 * 2 |
                                          temp_pixel.Green << 8 * 1 | temp_pixel.Blue << 8 * 0;

                        *(pic_buffer + width * row + col / 4) = temp_color_data;
                    }
                }

                displayer->font_pic_buffer[i] = pic_buffer;

                free(imageData);
                fclose(fp);
            }
        }
    }

    displayer->number_pic_max_length.x = 0;
    displayer->number_pic_max_length.y = 0;
    for (int i = 0; i <= 9; i++) {

        if (strlen(displayer->number_pic_path[i]) != 0) {
            FILE* fp;
            if ((fp = fopen(displayer->number_pic_path[i], "rb")) == NULL) {
                perror("can not open file!");
            }
            else {
                bitmapFileHeader bfHeader;
                fread(&bfHeader, 14, 1, fp);
                bitmapInfoHeader biHeader;
                fread(&biHeader, 40, 1, fp);

                int imSize   = biHeader.biSizeImage;
                int width    = biHeader.biWidth;
                int height   = biHeader.biHeight;
                int bitCount = biHeader.biBitCount;

                displayer->number_pic_length[i].x = width;
                displayer->number_pic_length[i].y = height;


                if (displayer->number_pic_max_length.x < width) {
                    displayer->number_pic_max_length.x = width;
                }
                if (displayer->font_pic_max_length.y < height) {
                    displayer->font_pic_max_length.y = height;
                }
                // printf("number:%d,width=%d\n", i, width);
                // printf("number cur max width=%d\n", displayer->number_pic_max_length.x);

                // 因为是32bit所以无所谓
                int line_width = (width + 3) / 4 * 4;
                int line_bytes = line_width * bitCount / 8;

                unsigned int* pic_buffer = malloc(width * height * sizeof(unsigned int));


                // 这里bfReserved2指向了offsetBits数据
                fseek(fp, bfHeader.bfOffBits, SEEK_SET);
                unsigned char* imageData = (unsigned char*)malloc(imSize * sizeof(unsigned char));
                memset(imageData, 0, imSize * sizeof(unsigned char));
                fread(imageData, imSize * sizeof(unsigned char), 1, fp);


                int          row, col;
                Pixel        temp_pixel;
                unsigned int temp_color_data;

                for (row = 0; row < height; row++) {
                    for (col = 0; col < width * 4; col += 4) {
                        temp_pixel.Blue  = *(imageData + line_bytes * (height - 1 - row) + col + 0);
                        temp_pixel.Green = *(imageData + line_bytes * (height - 1 - row) + col + 1);
                        temp_pixel.Red   = *(imageData + line_bytes * (height - 1 - row) + col + 2);
                        temp_pixel.Alpha = *(imageData + line_bytes * (height - 1 - row) + col + 3);


                        temp_color_data = temp_pixel.Alpha << 8 * 3 | temp_pixel.Red << 8 * 2 |
                                          temp_pixel.Green << 8 * 1 | temp_pixel.Blue << 8 * 0;

                        *(pic_buffer + width * row + col / 4) = temp_color_data;
                    }
                }

                displayer->number_pic_buffer[i] = pic_buffer;

                free(imageData);
                fclose(fp);
            }
        }
    }
    displayer->pic_max_height =
        displayer->font_pic_max_length.y > displayer->number_pic_max_length.y
            ? displayer->font_pic_max_length.y
            : displayer->number_pic_max_length.y;

    displayer->whole_width =
        (displayer->number_pic_max_length.x * 2 + displayer->font_pic_max_length.x + 5) * 3 - 5;

    displayer->whole_height = displayer->pic_max_height * 2 + 20;

    // printf("max font width:%d,max number width:%d\n",
    //        displayer->font_pic_max_length.x,
    //        displayer->number_pic_max_length.x);
    // printf("whole width=%d,whole height=%d\n", displayer->whole_width, displayer->whole_height);
}


void bmpTimeDisplayer_draw(struct bmpTimeDisplayer* draw_displayer)
{
    if (draw_displayer->need_redraw == true) {
        draw_displayer->need_redraw = false;

        // printf("timer drawing\n");

        painter_clear_range(draw_displayer->center_cord.x - draw_displayer->whole_width / 2,
                            draw_displayer->center_cord.y - draw_displayer->whole_height / 2,
                            draw_displayer->whole_width,
                            draw_displayer->whole_height);

        int left_x;
        int left_up_center_y;
        int left_down_center_y;

        left_x           = draw_displayer->center_cord.x - draw_displayer->whole_width / 2;
        left_up_center_y = draw_displayer->center_cord.y - draw_displayer->whole_height / 2 +
                           draw_displayer->pic_max_height / 2 + 10;
        left_down_center_y = draw_displayer->center_cord.y + draw_displayer->whole_height / 2 -
                             draw_displayer->pic_max_height / 2 - 10;

        int year  = draw_displayer->initiate_time.tm_year % 100;
        int mount = draw_displayer->initiate_time.tm_mon + 1;
        int day   = draw_displayer->initiate_time.tm_mday;

        // printf("year=%d,mon=%d,day=%d\n", year, mount, day);

        int cur_left_x = left_x;
        for (int i = 0; i <= 1; i++) {
            // printf("year i=%d:%d\n", i, year / ((i == 0) ? 10 : 1) % 10);
            // printf("width=%d,height=%d\n",
            //        draw_displayer->number_pic_length[year / ((i == 0) ? 10 : 1) % 10].x,
            //        draw_displayer->number_pic_length[year / ((i == 0) ? 10 : 1) % 10].y);
            painter_draw_ARGB_pic(
                draw_displayer->number_pic_buffer[year / ((i == 0) ? 10 : 1) % 10],
                cur_left_x,
                left_up_center_y -
                    draw_displayer->number_pic_length[year / ((i == 0) ? 10 : 1) % 10].y / 2,
                draw_displayer->number_pic_length[year / ((i == 0) ? 10 : 1) % 10].x,
                draw_displayer->number_pic_length[year / ((i == 0) ? 10 : 1) % 10].y);

            cur_left_x += draw_displayer->number_pic_max_length.x + 5;
        }
        painter_draw_ARGB_pic(draw_displayer->font_pic_buffer[0],
                              cur_left_x,
                              left_up_center_y - draw_displayer->font_pic_length[0].y / 2,
                              draw_displayer->font_pic_length[0].x,
                              draw_displayer->font_pic_length[0].y);
        cur_left_x += draw_displayer->font_pic_max_length.x + 5;



        for (int i = 0; i <= 1; i++) {
            painter_draw_ARGB_pic(
                draw_displayer->number_pic_buffer[mount / ((i == 0) ? 10 : 1) % 10],
                cur_left_x,
                left_up_center_y -
                    draw_displayer->number_pic_length[mount / ((i == 0) ? 10 : 1) % 10].y / 2,
                draw_displayer->number_pic_length[mount / ((i == 0) ? 10 : 1) % 10].x,
                draw_displayer->number_pic_length[mount / ((i == 0) ? 10 : 1) % 10].y);

            cur_left_x += draw_displayer->number_pic_max_length.x + 5;
        }
        painter_draw_ARGB_pic(draw_displayer->font_pic_buffer[1],
                              cur_left_x,
                              left_up_center_y - draw_displayer->font_pic_length[1].y / 2,
                              draw_displayer->font_pic_length[1].x,
                              draw_displayer->font_pic_length[1].y);
        cur_left_x += draw_displayer->font_pic_max_length.x + 5;



        for (int i = 0; i <= 1; i++) {
            painter_draw_ARGB_pic(
                draw_displayer->number_pic_buffer[day / ((i == 0) ? 10 : 1) % 10],
                cur_left_x,
                left_up_center_y -
                    draw_displayer->number_pic_length[day / ((i == 0) ? 10 : 1) % 10].y / 2,
                draw_displayer->number_pic_length[day / ((i == 0) ? 10 : 1) % 10].x,
                draw_displayer->number_pic_length[day / ((i == 0) ? 10 : 1) % 10].y);

            cur_left_x += draw_displayer->number_pic_max_length.x + 5;
        }
        painter_draw_ARGB_pic(draw_displayer->font_pic_buffer[2],
                              cur_left_x,
                              left_up_center_y - draw_displayer->font_pic_length[2].y / 2,
                              draw_displayer->font_pic_length[2].x,
                              draw_displayer->font_pic_length[2].y);



        int hour = draw_displayer->cur_real_time.tm_hour;
        int min  = draw_displayer->cur_real_time.tm_min;
        int sec  = draw_displayer->cur_real_time.tm_sec;

        cur_left_x = left_x;

        for (int i = 0; i <= 1; i++) {
            painter_draw_ARGB_pic(
                draw_displayer->number_pic_buffer[hour / ((i == 0) ? 10 : 1) % 10],
                cur_left_x,
                left_down_center_y -
                    draw_displayer->number_pic_length[hour / ((i == 0) ? 10 : 1) % 10].y / 2,
                draw_displayer->number_pic_length[hour / ((i == 0) ? 10 : 1) % 10].x,
                draw_displayer->number_pic_length[hour / ((i == 0) ? 10 : 1) % 10].y);

            cur_left_x += draw_displayer->number_pic_max_length.x + 5;
        }
        painter_draw_ARGB_pic(draw_displayer->font_pic_buffer[3],
                              cur_left_x,
                              left_down_center_y - draw_displayer->font_pic_length[3].y / 2,
                              draw_displayer->font_pic_length[3].x,
                              draw_displayer->font_pic_length[3].y);
        cur_left_x += draw_displayer->font_pic_max_length.x + 5;

        for (int i = 0; i <= 1; i++) {
            painter_draw_ARGB_pic(
                draw_displayer->number_pic_buffer[min / ((i == 0) ? 10 : 1) % 10],
                cur_left_x,
                left_down_center_y -
                    draw_displayer->number_pic_length[min / ((i == 0) ? 10 : 1) % 10].y / 2,
                draw_displayer->number_pic_length[min / ((i == 0) ? 10 : 1) % 10].x,
                draw_displayer->number_pic_length[min / ((i == 0) ? 10 : 1) % 10].y);

            cur_left_x += draw_displayer->number_pic_max_length.x + 5;
        }
        painter_draw_ARGB_pic(draw_displayer->font_pic_buffer[4],
                              cur_left_x,
                              left_down_center_y - draw_displayer->font_pic_length[4].y / 2,
                              draw_displayer->font_pic_length[4].x,
                              draw_displayer->font_pic_length[4].y);
        cur_left_x += draw_displayer->font_pic_max_length.x + 5;

        for (int i = 0; i <= 1; i++) {
            painter_draw_ARGB_pic(
                draw_displayer->number_pic_buffer[sec / ((i == 0) ? 10 : 1) % 10],
                cur_left_x,
                left_down_center_y -
                    draw_displayer->number_pic_length[sec / ((i == 0) ? 10 : 1) % 10].y / 2,
                draw_displayer->number_pic_length[sec / ((i == 0) ? 10 : 1) % 10].x,
                draw_displayer->number_pic_length[sec / ((i == 0) ? 10 : 1) % 10].y);

            cur_left_x += draw_displayer->number_pic_max_length.x + 5;
        }
        painter_draw_ARGB_pic(draw_displayer->font_pic_buffer[5],
                              cur_left_x,
                              left_down_center_y - draw_displayer->font_pic_length[5].y / 2,
                              draw_displayer->font_pic_length[5].x,
                              draw_displayer->font_pic_length[5].y);
    }
}


void bmpTimeDisplayer_update(struct bmpTimeDisplayer* draw_displayer, int delta_time)
{

    draw_displayer->cur_time = time(NULL);

    draw_displayer->cur_tick_time = difftime(draw_displayer->cur_time, draw_displayer->start_time);
    // printf("cur tick time=%f\n", draw_displayer->cur_tick_time);
    // printf("pre tick time=%f\n", draw_displayer->pre_tick_time);
    if (draw_displayer->cur_tick_time - draw_displayer->pre_tick_time >= 1.0f) {
        // printf("timer updating\n");
        draw_displayer->need_redraw   = true;
        draw_displayer->pre_tick_time = draw_displayer->cur_tick_time;

        int diff_hour = (int)draw_displayer->cur_tick_time / 3600;
        int diff_min  = ((int)draw_displayer->cur_tick_time % 3600) / 60;
        int diff_sec  = ((int)draw_displayer->cur_tick_time % 60);

        printf("cur tick time=%f\n", draw_displayer->cur_tick_time);
        printf("diff hour=%d,min=%d,sec=%d\n", diff_hour, diff_min, diff_sec);

        // draw_displayer->cur_real_time.tm_sec  = draw_displayer->initiate_time.tm_sec + diff_sec;
        // draw_displayer->cur_real_time.tm_min  = draw_displayer->initiate_time.tm_min + diff_min;
        // draw_displayer->cur_real_time.tm_hour = draw_displayer->initiate_time.tm_hour +
        // diff_hour;
        draw_displayer->cur_real_time.tm_sec  = draw_displayer->initiate_time.tm_sec;
        draw_displayer->cur_real_time.tm_min  = draw_displayer->initiate_time.tm_min;
        draw_displayer->cur_real_time.tm_hour = draw_displayer->initiate_time.tm_hour;

        printf("initiate time:%d,%d,%d\n",
               draw_displayer->initiate_time.tm_hour,
               draw_displayer->initiate_time.tm_min,
               draw_displayer->initiate_time.tm_sec);


        draw_displayer->cur_real_time.tm_sec += diff_sec;
        if (draw_displayer->cur_real_time.tm_sec >= 60) {
            draw_displayer->cur_real_time.tm_sec %= 60;
            draw_displayer->cur_real_time.tm_min += 1;
            if (draw_displayer->cur_real_time.tm_min >= 60) {
                draw_displayer->cur_real_time.tm_min %= 60;
                draw_displayer->cur_real_time.tm_hour += 1;
                if (draw_displayer->cur_real_time.tm_hour >= 24)
                    draw_displayer->cur_real_time.tm_hour %= 24;
            }
        }
        draw_displayer->cur_real_time.tm_min += diff_min;
        if (draw_displayer->cur_real_time.tm_min >= 60) {
            draw_displayer->cur_real_time.tm_min %= 60;
            draw_displayer->cur_real_time.tm_hour += 1;
            if (draw_displayer->cur_real_time.tm_hour >= 24)
                draw_displayer->cur_real_time.tm_hour %= 24;
        }
        draw_displayer->cur_real_time.tm_hour += diff_hour;
        if (draw_displayer->cur_real_time.tm_hour >= 24) draw_displayer->cur_real_time.tm_hour = 0;

        printf("cur real time:%d:%d:%d\n",
               draw_displayer->cur_real_time.tm_hour,
               draw_displayer->cur_real_time.tm_min,
               draw_displayer->cur_real_time.tm_sec);
    }
}
