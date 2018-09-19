#include <jni.h>
#include <string>
#include <android/bitmap.h>
#include <android/log.h>
#include <malloc.h>
//#include "jpeglib.h"
extern "C"{
#include "inc/jpeglib.h"
}

#define LOG_TAG "gaof"
#define  LOGE(...) _android_log_print(ANDROID_LOG_ERROR,LOG_TAG,_VA_ARGS_)
#define ture 1
typedef uint8_t BYTE;


void writeImg(BYTE *data, const char *path, int w, int h);


void writeImg(BYTE *data, const char *path, int w, int h) {
//    jpeg库
//    初始化
    struct jpeg_compress_struct jpeg_struct;
    jpeg_error_mgr err;
    //设置错误处理函数
    jpeg_struct.err=jpeg_std_error(&err);
    //jpeg_struct 初始化
    jpeg_create_compress(&jpeg_struct);
    FILE *file=fopen(path,"wb");
    //设置输出路径
    jpeg_stdio_dest(&jpeg_struct,file);

    jpeg_struct.image_height=h;
    jpeg_struct.image_width=w;
    //skia默认不打开哈夫曼压缩  FALSE为哈夫曼
    jpeg_struct.arith_code=FALSE;
    //优化编码
    jpeg_struct.optimize_coding=TRUE;
    jpeg_struct.in_color_space=JCS_RGB;
    //rgb组成部分3
    jpeg_struct.input_components=3;
    //设置默认
    jpeg_set_defaults(&jpeg_struct);

//    开始压缩
    jpeg_start_compress(&jpeg_struct,TRUE);

//    一行行写入
    //next_scanline/* 0 .. image_height-1  */
    // JSAMPARRAY
    JSAMPROW  row_pointer[1];
    int row_strode=w*3;
    while (jpeg_struct.next_scanline<h){
        row_pointer[0] = &data[jpeg_struct.next_scanline*row_strode];
        jpeg_write_scanlines(&jpeg_struct,row_pointer,1);
    }

//    结束压缩
    jpeg_finish_compress(&jpeg_struct);
    jpeg_destroy_compress(&jpeg_struct);
    fclose(file);

}

extern "C"
JNIEXPORT void JNICALL
Java_com_gaof_compress_Engine_compressHuffman(JNIEnv *env, jobject instance, jobject bitmap,
                                               jstring path_) {
    const char *path = env->GetStringUTFChars(path_, 0);
    AndroidBitmapInfo bitmapInfo;
    AndroidBitmap_getInfo(env,bitmap,&bitmapInfo);
    BYTE *pixels;
    //图片转为像素
    AndroidBitmap_lockPixels(env,bitmap,(void **)&pixels);
    int h=bitmapInfo.height;
    int w=bitmapInfo.width;
    //像素数组  rgb 数组
    BYTE *data,*tmpData;
    data=(BYTE *)malloc(w*h*3);
    tmpData=data;
    BYTE r,g,b;
    int color;
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            color=*((int *)pixels);
            r=((color&0x00FF0000) >> 16);
            g=((color&0x0000FF00) >> 8);
            b=((color&0x000000FF));
            *data=b;
            *(data+1)=g;
            *(data+2)=r;
            data+=3;
            pixels+=4;
        }
//        opencv jpeg  排列bgr
    }
    AndroidBitmap_unlockPixels(env,bitmap);
    //data数组 rgb数据
    writeImg(tmpData,path,w,h);

    env->ReleaseStringUTFChars(path_, path);
}