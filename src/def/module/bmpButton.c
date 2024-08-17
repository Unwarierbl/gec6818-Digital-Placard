#include "module/bmpButton.h"
#include "painter.h"

#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include "decTypes.h"

void init_bmpButton(struct bmpButton* button)
{
    button->center_cord.x = 0;
    button->center_cord.y = 0;

    button->cur_press_status      = RELEASE;
    button->pre_draw_press_status = PRESS;


    button->handle_release = NULL;
}


struct bmpButton* request_bmpButton_direct()
{
    struct bmpButton* new_button = (struct bmpButton*)malloc(sizeof(struct bmpButton));
    init_bmpButton(new_button);

    return new_button;
}

void remove_bmpButton_direct(struct bmpButton* remove_button)
{
    if (remove_button->press_pic_buffer != NULL) {
        free(remove_button->press_pic_buffer);
    }
    if (remove_button->release_pic_buffer != NULL) {
        free(remove_button->release_image_height);
    }

    free(remove_button);
}

void bmpButton_set_press_pic_path(struct bmpButton* button, const char* new_path)
{
    memset(button->press_pic_path, 0, sizeof(button->press_pic_path));
    strncpy(button->press_pic_path, new_path, sizeof(button->press_pic_path));
}

void bmpButton_set_release_pic_path(struct bmpButton* button, const char* new_path)
{
    memset(button->release_pic_path, 0, sizeof(button->release_pic_path));
    strncpy(button->release_pic_path, new_path, sizeof(button->release_pic_path));
}


void bmpButton_load_pic(struct bmpButton* bmpButton)
{
    if (strlen(bmpButton->press_pic_path) != 0) {
        FILE* fp;
        if ((fp = fopen(bmpButton->press_pic_path, "rb")) == NULL) {
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

            bmpButton->press_image_width  = width;
            bmpButton->press_image_height = height;

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

            bmpButton->press_pic_buffer = pic_buffer;

            free(imageData);
            fclose(fp);
        }
    }

    if (strlen(bmpButton->release_pic_path) != 0) {
        FILE* fp;
        if ((fp = fopen(bmpButton->release_pic_path, "rb")) == NULL) {
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

            bmpButton->release_image_width  = width;
            bmpButton->release_image_height = height;

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

            bmpButton->release_pic_buffer = pic_buffer;

            free(imageData);
            fclose(fp);
        }
    }
}

void bmpButton_draw(struct bmpButton* draw_button)
{
    if (draw_button->pre_draw_press_status != draw_button->cur_press_status) {
        switch (draw_button->cur_press_status) {
        case PRESS:
            if (draw_button->press_pic_buffer != NULL) {
                painter_clear_range(
                    draw_button->center_cord.x - draw_button->release_image_width / 2,
                    draw_button->center_cord.y - draw_button->release_image_height / 2,
                    draw_button->release_image_width,
                    draw_button->release_image_height);

                painter_draw_ARGB_pic_zoom(
                    draw_button->press_pic_buffer,
                    draw_button->center_cord.x - draw_button->press_image_width / 2,
                    draw_button->center_cord.y - draw_button->press_image_height / 2,
                    draw_button->press_image_width,
                    draw_button->press_image_height,
                    1);
            }
            break;
        case RELEASE:
            if (draw_button->release_pic_buffer != NULL) {
                painter_clear_range(
                    draw_button->center_cord.x - draw_button->press_image_width / 2,
                    draw_button->center_cord.y - draw_button->press_image_height / 2,
                    draw_button->press_image_width,
                    draw_button->press_image_height);

                painter_draw_ARGB_pic_zoom(
                    draw_button->release_pic_buffer,
                    draw_button->center_cord.x - draw_button->release_image_width / 2,
                    draw_button->center_cord.y - draw_button->release_image_height / 2,
                    draw_button->release_image_width,
                    draw_button->release_image_height,
                    1);
            }

            break;
        default: break; ;
        }
        draw_button->pre_draw_press_status = draw_button->cur_press_status;
    }
}

void bmpButton_analyze_touch(struct bmpButton* button, touchStatusData touchScreen_data)
{
    vec2 touch_cord = touchScreen_data.cur_touch_cord;

    switch (touchScreen_data.touch_status) {
    case PRESS:
        // printf("touch:(%d,%d) this:(%d,%d)(%d,%d)\n",
        //        touch_cord.x,
        //        touch_cord.y,
        //        button->center_cord.x,
        //        button->center_cord.y,
        //        button->press_image_width,
        //        button->press_image_height);
        if (touch_cord.x >= button->center_cord.x - button->press_image_width / 2 &&
            touch_cord.x <= button->center_cord.x + button->press_image_width / 2 &&
            touch_cord.y >= button->center_cord.y - button->press_image_height / 2 &&
            touch_cord.y <= button->center_cord.y + button->press_image_height / 2) {

            button->cur_press_status = PRESS;
        }
        break;
    case RELEASE:
        if (button->cur_press_status == PRESS) {
            if (touch_cord.x >= button->center_cord.x - button->release_image_width / 2 &&
                touch_cord.x <= button->center_cord.x + button->release_image_width / 2 &&
                touch_cord.y >= button->center_cord.y - button->release_image_height / 2 &&
                touch_cord.y <= button->center_cord.y + button->release_image_height / 2) {

                button->cur_press_status = RELEASE;
                if (button->handle_release != NULL) {
                    button->handle_release(button);
                }
            }
        }
        break;
    default: break;
    }
}
