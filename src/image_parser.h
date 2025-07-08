#ifndef PARSER_H
#define PARSER_H

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




void image_reader(BitMapFileHeader* bmpFileHeader, BitMapInfoHeader* bmpInfoHeader, const char* path_to_file, unsigned char* pixel_buffer);



#endif
