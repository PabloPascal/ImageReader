#include "../src/image_parser.h"
#include <stdio.h>

int main(){
     
    BitMapFileHeader bmFileHeader;
    BitMapInfoHeader bmInfoHeader;    
   

    const char* path = "../images/Tiffany.bmp";
    unsigned char* pixel_buf;
    
    image_reader(&bmFileHeader, &bmInfoHeader, path, &pixel_buf);



    size_t height = bmInfoHeader.biHeight;
    size_t width = bmInfoHeader.biWidth;
    //inverse_color(pixel_buf, bmInfoHeader.biWidth, bmInfoHeader.biHeight);
    printf("mean brightness: %f", meanBrightnessPixels(pixel_buf, width, height));
    grayFilter(pixel_buf, width, height, 0.5);
    outputImage_inFile("../output/", "output.bmp", &bmFileHeader, &bmInfoHeader, pixel_buf);


    
    return 0;
}
