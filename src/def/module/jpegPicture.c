#include "module/jpegPicture.h"
#include "painter.h"

#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include <jpeglib.h>
#include <setjmp.h>


struct my_error_mgr
{
    struct jpeg_error_mgr pub; /* "public" fields */

    jmp_buf setjmp_buffer; /* for return to caller */
};

typedef struct my_error_mgr* my_error_ptr;

METHODDEF(void)
my_error_exit(j_common_ptr cinfo)
{
    /* cinfo->err really points to a my_error_mgr struct, so coerce pointer */
    my_error_ptr myerr = (my_error_ptr)cinfo->err;

    /* Always display the message. */
    /* We could postpone this until after returning, if we chose. */
    (*cinfo->err->output_message)(cinfo);

    /* Return control to the setjmp point */
    longjmp(myerr->setjmp_buffer, 1);
}


// ----------------------------------------------------------------


void init_jpegPic(struct jpegPicture* jpeg)
{
    jpeg->center_cord.x = 0;
    jpeg->center_cord.y = 0;

    jpeg->scale = 1.0f;
}
void remove_jpegPic_direct(struct jpegPicture* remove_jpeg)
{
    if (remove_jpeg->pic_buffer != NULL) {
        free(remove_jpeg->pic_buffer);
    }
    free(remove_jpeg);
}

// 注意这里并不是结点
struct jpegPicture* request_jpegPic_direct()
{
    struct jpegPicture* new_jpeg = (struct jpegPicture*)malloc(sizeof(struct jpegPicture));
    init_jpegPic(new_jpeg);

    return new_jpeg;
}

void jpegPic_set_pic_path(struct jpegPicture* jpeg, const char* new_path)
{
    memset(jpeg->pic_path, 0, sizeof(jpeg->pic_path));
    strncpy(jpeg->pic_path, new_path, sizeof(jpeg->pic_path));
}


// 结果保存在 pic_buffer 中
void jpegPic_load_pic(struct jpegPicture* jpeg)
{
    // 创建解码对象，其是一个结构体变量
    struct jpeg_decompress_struct cinfo;
    // 创建错误处理对象
    struct my_error_mgr jerr;

    /*[2]：打开待解码的jpg图片，使用fopen的时候需要添加选项”b”，以二进制方式打开文件！*/

    FILE*         infile;         // 接收打开文件的文件指针
    char*         line_buffer;    // 输出行缓冲区
    unsigned int* whole_buffer;   // 输出总缓冲区
    int           row_stride;     // buffer一行的像素点数量，即图片的宽度

    // 以二进制方式打开图片，并进行错误处理
    if ((infile = fopen(jpeg->pic_path, "rb")) == NULL) {
        fprintf(stderr, "can't open %s\n", jpeg->pic_path);
        return;
    }

    // 将错误处理对象与解码对象相关联
    cinfo.err           = jpeg_std_error(&jerr.pub);
    jerr.pub.error_exit = my_error_exit;

    if (setjmp(jerr.setjmp_buffer)) {
        jpeg_destroy_decompress(&cinfo);
        fclose(infile);
        return;
    }

    jpeg_create_decompress(&cinfo);

    // 把打开的文件的文件指针和解码对象进行绑定
    jpeg_stdio_src(&cinfo, infile);

    /*[3]：读取待解码图片的文件头，并把图像信息和解码对象进行关联，通过解码对象对jpg图片进行解码*/

    (void)jpeg_read_header(&cinfo, TRUE);

    /*[5]：开始对jpg图片进行解码，调用函数之后开始解码，可以得到图像宽、图像高等信息！*/
    // 我们只需要调用该函数，将图像信息放入解码对象中，无需注意其的返回值
    (void)jpeg_start_decompress(&cinfo);

    /*[6]：开始设计一个循环，在循环中每次读取1行的图像数据，并写入到LCD中*/
    // 计算图像一行的大小
    jpeg->image_width  = cinfo.output_width;
    jpeg->image_height = cinfo.output_height;

    row_stride = cinfo.output_width * cinfo.output_components;
    // 为自定义缓冲区申请堆内存，注意申请的内存空间大小应为图像一行的大小


    line_buffer  = calloc(1, row_stride);
    whole_buffer = calloc(cinfo.output_height, row_stride);



    // 定义一个int类型变量，用于存放颜色分量数据
    int data = 0;
    /*定义一个循环，用于循环写入一行的图像数据；
    使用解码对象当前扫描行数与图像的高比较结果作为循环条件，当两者相等，即图像数据写入完后退出循环*/
    while (cinfo.output_scanline < cinfo.output_height) {
        /*调用jpeg_read_scanlines函数，读取解码对象中的图像一行数据，并存放进自定义缓冲区中
        且cinfo.output_scanline会随着调用该函数而增加1，保证while循环能够正常退出*/
        (void)jpeg_read_scanlines(&cinfo, &line_buffer, 1);   // 从上到下，从左到右  RGB RGB RGB RGB

        // 将缓冲区中存储的数据逐一写入LCD的内存映射空间中
        for (int i = 0; i < cinfo.output_width; ++i)   // 012 345
        {
            /*由于图片没有透明度，所以一个像素点大小为3byte，而data为int类型变量，所以需要
            借助"|=" 使得颜色分量顺序存储正确；又因为JEPG存储颜色分量顺序为RGB，所以进行下面算法*/
            data |= line_buffer[3 * i] << 16;      // Red
            data |= line_buffer[3 * i + 1] << 8;   // Green
            data |= line_buffer[3 * i + 2];        // Blue

            /*把像素点写入到LCD的指定位置。其中800*start_y +
            start_x控制的是用户自定义的图片显示初始位置；
            800*(cinfo.output_scanline-1)控制的是写入图像数据的行数切换；+
            i控制的是写入图像数据的列数切换*/
            *(whole_buffer + cinfo.output_scanline * cinfo.output_width + i) = data;

            // 最后需将data内部清零，避免对下一次循环的颜色分量写入造成影响
            data = 0;
        }
    }

    jpeg->pic_buffer = whole_buffer;

    jpeg_finish_decompress(&cinfo);

    jpeg_destroy_decompress(&cinfo);

    fclose(infile);
}


void jpegPic_draw(struct jpegPicture* draw_jpeg)
{
    if (draw_jpeg->pic_buffer == NULL) {
        return;
    }

    painter_clear_range(draw_jpeg->center_cord.x - draw_jpeg->image_width / 2,
                        draw_jpeg->center_cord.y - draw_jpeg->image_height / 2,
                        draw_jpeg->image_width,
                        draw_jpeg->image_height);

    painter_draw_RGB_pic_zoom(draw_jpeg->pic_buffer,
                              draw_jpeg->center_cord.x - draw_jpeg->image_width / 2,
                              draw_jpeg->center_cord.y - draw_jpeg->image_height / 2,
                              draw_jpeg->image_width,
                              draw_jpeg->image_height,
                              draw_jpeg->scale);
}
