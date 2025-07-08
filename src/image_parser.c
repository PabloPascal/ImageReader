#include "../src/image_parser.h"

#include <stdio.h>
#include <stdlib.h>



void image_reader(BitMapFileHeader* bmpFileHeader, BitMapInfoHeader* bmpInfoHeader, const char* path_to_file, unsigned char* pixel_buffer){
    
    FILE* file = fopen(path_to_file, "rb");
    if(!file){
        perror("cannot open the file! with path: ");
        printf("%s", path_to_file);
        return;
    }

    if(!fread(bmpFileHeader, sizeof(BitMapFileHeader), 1, file) || 
       !fread(bmpInfoHeader, sizeof(BitMapInfoHeader), 1, file)){
        perror("cannot read from file\n");
        fclose(file);
        return;
    }


    if(!bmpFileHeader->bfType[0] == 'B' || !bmpFileHeader->bfType[1] == 'M'){
        perror("not a BM format file!\n");
        fclose(file);
        return;
    }   

    fseek(file, bmpFileHeader->bfOffbits, SEEK_SET);

    size_t height = bmpInfoHeader->biHeight;
    size_t width = bmpInfoHeader->biWidth;
    size_t rowSize = 3 * width; //т.к в bmp файле до 3х цветовых каналов
    int padding = (4 - (rowSize % 4)) % 4;


    pixel_buffer = malloc(height * rowSize); 
    if(!pixel_buffer){
        perror("memory allocate fall\n");
        return;
    }   

    for(int y = height - 1; y >= 0; y--){
        unsigned char* row = pixel_buffer + y * rowSize;
        if(fread(row, 1, rowSize, file) != rowSize){
            perror("errror reading pixel data");
            free(pixel_buffer);
            fclose(file);
            return;
        }
        //пропускаем байты выравнивания
        fseek(file, padding, SEEK_CUR);

    }

    fclose(file);

}




