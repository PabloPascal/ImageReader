#include "../src/image_parser.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void image_reader(BitMapFileHeader* bmpFileHeader, BitMapInfoHeader* bmpInfoHeader, const char* path_to_file,
    unsigned char** pixel_buffer){
    
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


    (*pixel_buffer) = malloc(height * rowSize); 
    if(!pixel_buffer){
        perror("memory allocate fall\n");
        return;
    }   

    for(int y = 0; y < height; y++){
        unsigned char* row = (*pixel_buffer) + y * rowSize;
        if(fread(row, 1, rowSize, file) != rowSize){
            perror("errror reading pixel data");
            free(*pixel_buffer);
            fclose(file);
            return;
        }
        //пропускаем байты выравнивания
        fseek(file, padding, SEEK_CUR);

    }

    fclose(file);

}



void inverse_color(unsigned char* pixel_buffer,const size_t image_width,const size_t image_height){

    size_t rowLength = 3*image_width;

    for(size_t y = 0; y < image_height; y++){
        unsigned char* row = pixel_buffer + rowLength * y;
        for(size_t x = 0; x < rowLength; x++){
            row[x] = 255 - row[x];
        }

    }

}


void outputImage_inFile(const char* path, const char* nameWithExtention, 
            BitMapFileHeader* bmpFileHeader, BitMapInfoHeader* bmpInfoHeader, 
            const unsigned char* input_pixel_buffer){

    size_t len_name = strlen(nameWithExtention);

    if(!nameWithExtention[len_name-3] == 'b' || !nameWithExtention[len_name-2] == 'm' || 
        !nameWithExtention[len_name-2] == 'p'){
        perror("not a bmp format!\n");
        return;
    }

    FILE* outputFile = fopen(nameWithExtention, "wb");
    
    fwrite(bmpFileHeader, sizeof(BitMapFileHeader), 1, outputFile);
    fwrite(bmpInfoHeader, sizeof(BitMapInfoHeader), 1, outputFile);

    size_t image_width = bmpInfoHeader->biWidth;
    size_t image_height = bmpInfoHeader->biHeight;

    fwrite(input_pixel_buffer, 3 * sizeof(unsigned char), image_width * image_height, outputFile);
    
    fclose(outputFile);

}


void grayFilter(unsigned char* pixel_buffer,const size_t image_width,const size_t image_height, float koefficient){

    //newPixelLight = oldPixelLight + k * (oldPixelLight - meanLight)
    size_t rowLength = 3 * image_width;

    float pixelBrightness = 0;
    float meanLight = meanBrightnessPixels(pixel_buffer, image_width, image_height);
    float newPixelBrightness = 0;
    Pixel pixel;

    float kr = 0.2126;
    float kg = 0.7152;
    float kb = 0.0722;

    for(size_t v = 0; v < image_height; v++){
        unsigned char* row = pixel_buffer + rowLength * v;
        for(size_t u = 0; u < rowLength; u+=3){

            pixelBrightness = getPixelBrighntess(pixel_buffer, image_width, u, v);
            newPixelBrightness = pixelBrightness + koefficient * (pixelBrightness - meanLight);
            pixel.r = newPixelBrightness;
            pixel.g = newPixelBrightness;
            pixel.b = newPixelBrightness;

            setPixel(pixel_buffer, image_width, u, v, pixel);

        }

    }



}


float meanBrightnessPixels(const unsigned char* pixel_buffer, 
                            const size_t image_width,const size_t image_height)
{
    float brightness_mean = 0;
    unsigned int brightness_sum = 0;
    float brightness = 0;

    size_t rowLength = 3 * image_width;

    for(size_t y = 0; y < image_height; y++){
        const unsigned char* row = pixel_buffer + rowLength * y;
        for(size_t x = 0; x < rowLength; x+=3){
            brightness = getPixelBrighntess(pixel_buffer, image_width, x, y);
            brightness_sum += brightness;
        }

    }

    brightness_mean = brightness_sum  / (float)(image_width * image_height);
    
    return brightness_mean;
}



float getPixelBrighntess(const unsigned char* pixels, const size_t image_width, size_t u, size_t v)
{
    float kr = 0.299;
    float kg = 0.587 ;
    float kb = 0.114 ;
    
    unsigned int r = pixels[v * 3 * image_width + u];
    unsigned int g = pixels[v * 3 * image_width + u + 1];
    unsigned int b  = pixels[v * 3 * image_width + u + 2];

    int brightness = (int)(kr * r + kg * g + kb * b);
    return brightness;

}



void setPixel(unsigned char* pixels, const size_t image_width, size_t u, size_t v, Pixel pixel){
    pixels[v * 3 * image_width + u + 0] = pixel.r;
    pixels[v * 3* image_width + u + 1] = pixel.g;
    pixels[v * 3 * image_width + u + 2] = pixel.b;
}
