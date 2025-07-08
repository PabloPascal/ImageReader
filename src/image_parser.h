#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <stdlib.h>

#pragma pack(push, 1)
typedef struct{
    char bfType[2]; //тип данных (нам нужно BM)
    int bfSize; //размер файла
    short bfReserved1;  
    short bfReserved2;
    int bfOffbits; //начало данных картинки (после хедеров)
}BitMapFileHeader;


typedef struct{
    int biSize; //размер нашей структуры
    int biWidth; //ширина картинки
    int biHeight; //высота картинки
    short biPlanes;
    short biBitCount; //количество битов в 1 пикселе
    int biCompression;
    int biSizeImage;
    int biXPelsPerMeter;
    int biYPelsPerMeter;
    int biClrUsed;
    int biClrImportant;
} BitMapInfoHeader;
#pragma pack(pop)


typedef struct PIXEL{
    unsigned int r;
    unsigned int g;
    unsigned int b;
} Pixel;



void image_reader(BitMapFileHeader* bmpFileHeader, BitMapInfoHeader* bmpInfoHeader, const char* path_to_file, unsigned char** pixel_buffer);

void outputImage_inFile(const char* path, const char* name, 
            BitMapFileHeader* bmpFileHeader, BitMapInfoHeader* bmpInfoHeader,
            const unsigned char* input_pixel_buffer);

void inverse_color(unsigned char* pixel_buffer,const size_t image_width,const size_t image_height);

void grayFilter(unsigned char* pixel_buffer,const size_t image_width,const size_t image_height, float koefficient);

float meanBrightnessPixels(const unsigned char* pixel_buffer,const size_t image_width,const size_t image_height);

float getPixelBrighntess(const unsigned char* pixels,const size_t image_width, size_t u, size_t v);

void setPixel(unsigned char* pixels, const size_t image_width, size_t u, size_t v, Pixel pixel);

#endif
