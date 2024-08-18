#include "module/bmpSlidePicList.h"
#include "painter.h"


void init_bmpSlidePicList_node(struct bmpSlidePicList* bmp_node)
{
    bmp_node->center_cord.x = 0;
    bmp_node->center_cord.y = 0;

    bmp_node->order       = 0;
    bmp_node->need_redraw = true;

    bmp_node->cur_display_order = 1;

    bmp_node->image_width  = 0;
    bmp_node->image_height = 0;

    INIT_LIST_HEAD(&bmp_node->list_node);
}

struct bmpSlidePicList* request_bmpSlidePicList_node_direct()
{
    struct bmpSlidePicList* new_node =
        (struct bmpSlidePicList*)malloc(sizeof(struct bmpSlidePicList));

    init_bmpSlidePicList_node(new_node);
    return new_node;
}

struct bmpSlidePicList* find_bmpSlidePicList_node(struct bmpSlidePicList* head_node, int find_order)
{
    if (list_empty(&head_node->list_node)) {
        return head_node;
    }

    struct bmpSlidePicList* pos;
    list_for_each_entry(pos, &head_node->list_node, list_node)
    {
        // printf("1\n");
        if (pos->order == find_order) {
            break;
        }
    }
    return pos;
}



void insert_bmpSlidePicList_node(struct bmpSlidePicList* head_node,
                                 struct bmpSlidePicList* insert_node)
{
    insert_node->order =   // 设置order,默认插在最后面
        list_entry(head_node->list_node.prev, struct bmpSlidePicList, list_node)->order + 1;

    insert_node->center_cord = head_node->center_cord;
    list_add_tail(&insert_node->list_node, &head_node->list_node);

    int whole_pic_numb = insert_node->order;

    head_node->image_width  = insert_node->image_width;
    head_node->image_height = insert_node->image_height;
}



void bmpSlidePicList_set_pic_path(struct bmpSlidePicList* bmp_node, const char* new_path)
{
    memset(bmp_node->pic_path, 0, sizeof(bmp_node->pic_path));
    strncpy(bmp_node->pic_path, new_path, sizeof(bmp_node->pic_path));
}


void bmpSlidePicList_load_pic(struct bmpSlidePicList* bmp_node)
{
    if (strlen(bmp_node->pic_path) == 0) {
        perror("bmp node path is empty!");
        return;
    }

    FILE* fp;
    if ((fp = fopen(bmp_node->pic_path, "rb")) == NULL) {
        fprintf(stderr, "can not open file :%s\n", bmp_node->pic_path);
        return;
    }

    bitmapFileHeader bfHeader;
    fread(&bfHeader, 14, 1, fp);
    bitmapInfoHeader biHeader;
    fread(&biHeader, 40, 1, fp);

    int imSize   = biHeader.biSizeImage;
    int width    = biHeader.biWidth;
    int height   = biHeader.biHeight;
    int bitCount = biHeader.biBitCount;

    bmp_node->image_width  = width;
    bmp_node->image_height = height;

    // printf("bmp bitCount=%d, width=%d, height=%d\n", bitCount, width, height);


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

    bmp_node->pic_buffer = pic_buffer;

    free(imageData);
    fclose(fp);
}


void bmpSlidePicList_draw(struct bmpSlidePicList* head_node)
{
    if (head_node->need_redraw == true) {
        painter_clear_range(head_node->center_cord.x - head_node->image_width / 2,
                            head_node->center_cord.y - head_node->image_height / 2,
                            head_node->image_width,
                            head_node->image_height);


        struct bmpSlidePicList* pos;

        pos = find_bmpSlidePicList_node(head_node, head_node->cur_display_order);
        if (pos == head_node) {
            pos = list_entry(head_node->list_node.next, struct bmpSlidePicList, list_node);
            head_node->cur_display_order = 1;
        }
        painter_draw_ARGB_pic(pos->pic_buffer,
                              head_node->center_cord.x - head_node->image_width / 2,
                              head_node->center_cord.y - head_node->image_height / 2,
                              head_node->image_width,
                              head_node->image_height);

        head_node->need_redraw = false;
    }
}
