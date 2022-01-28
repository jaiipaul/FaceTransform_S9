#include <cstdio>
#include <stdint.h>


enum ImageType {
	PNG, JPG, BMP, TGA
};


struct Image {
    uint8_t* data = NULL;
    size_t size = 0; 
    int w; 
    int h; 
    int channels; 

    Image(const char* filename, int channel_force); 
    Image(int w, int h, int channels); 
    Image(const Image& img) ;
    ~Image();

    bool read(const char* filename, int channel_force); 
    bool write(const char* filename);
    
    ImageType getFileType(const char* filename);
    Image& overlay(const Image& source, int x, int y);
    Image& resize( uint16_t x_new, uint16_t y_new);
};