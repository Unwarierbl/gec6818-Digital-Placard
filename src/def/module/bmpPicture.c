#include "module/bmpPicture.h"
#include "painter.h"



void init_bmpPicture(struct bmpPicture* bmp)
{
    bmp->center_cord.x = 0;
    bmp->center_cord.y = 0;

    bmp->image_width  = 0;
    bmp->image_height = 0;
}


struct bmpPicture* request_bmpPicture_direct()
{
    struct bmpPicture* new_bmp = (struct bmpPicture*)malloc(sizeof(struct bmpPicture));
    init_bmpPicture(new_bmp);

    return new_bmp;
}


void remove_bmpPicture_direct(struct bmpPicture* remove_bmp)
{
    if (remove_bmp->pic_buffer != NULL) {
        free(remove_bmp->pic_buffer);
    }
    free(remove_bmp);
}


void bmpPicture_set_pic_path(struct bmpPicture* bmp, const char* new_path)
{
    memset(bmp->pic_path, 0, sizeof(bmp->pic_path));
    strncpy(bmp->pic_path, new_path, sizeof(bmp->pic_path));
}

void bmpPicture_load_pic(struct bmpPicture* bmpPicture)
{
    if (strlen(bmpPicture->pic_path) != 0) {
        FILE* fp;
        if ((fp = fopen(bmpPicture->pic_path, "rb")) == NULL) {
            fprintf(stderr, "can not open file :%s", bmpPicture->pic_path);
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

            bmpPicture->image_width  = width;
            bmpPicture->image_height = height;

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

            bmpPicture->pic_buffer = pic_buffer;

            free(imageData);
            fclose(fp);
        }
    }
}

void bmpPicture_draw(struct bmpPicture* draw_bmp)
{
    if (draw_bmp->pic_buffer != NULL) {
        painter_clear_range(draw_bmp->center_cord.x - draw_bmp->image_width / 2,
                            draw_bmp->center_cord.y - draw_bmp->image_height / 2,
                            draw_bmp->image_width,
                            draw_bmp->image_height);

        painter_draw_ARGB_pic(draw_bmp->pic_buffer,
                              draw_bmp->center_cord.x - draw_bmp->image_width / 2,
                              draw_bmp->center_cord.y - draw_bmp->image_height / 2,
                              draw_bmp->image_width,
                              draw_bmp->image_height);
    }
}
