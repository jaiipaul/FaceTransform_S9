#define STB_IMAGE_IMPLEMENTATION 
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define BYTE_BOUND(value) value < 0 ? 0 : (value > 255 ? 255 : value)
#include "Image.h"
#include "stb_image.h"
#include "stb_image_write.h"
#include <string.h>
#include <math.h>
#include <iostream>

using namespace std; 



Image::Image(const char* filename, int channel_force) {
	if(read(filename, channel_force)) {
		printf("Read %s\n", filename);
		size = w*h*channels;
	}
	else {
		printf("Failed to read %s\n", filename);
	}
}

Image::Image(int w, int h, int channels) : w(w), h(h), channels(channels) {
	size = w*h*channels;
	data = (uint8_t*)calloc(size, sizeof(uint8_t));
	//data = new int[size];
}

Image::Image(const Image& img) : Image(img.w, img.h, img.channels) {
	memcpy(data, img.data, size);
}

Image::~Image() {
	stbi_image_free(data);
}

bool Image::read(const char* filename, int channel_force) {
	data = (uint8_t*)stbi_load(filename, &w, &h, &channels, channel_force);
	channels = channel_force == 0 ? channels : channel_force;
	return data != NULL;
}

bool Image::write(const char* filename) {
	ImageType type = getFileType(filename);
	int success;
  switch (type) {
    case PNG:
      success = stbi_write_png(filename, w, h, channels, data, w*channels);
      break;
    case BMP:
      success = stbi_write_bmp(filename, w, h, channels, data);
      break;
    case JPG:
      success = stbi_write_jpg(filename, w, h, channels, data, 100);
      break;
    case TGA:
      success = stbi_write_tga(filename, w, h, channels, data);
      break;
  }
  if(success != 0) {
    printf("\e[32mWrote \e[36m%s\e[0m, %d, %d, %d, %zu\n", filename, w, h, channels, size);
    return true;
  }
  else {
    printf("\e[31;1m Failed to write \e[36m%s\e[0m, %d, %d, %d, %zu\n", filename, w, h, channels, size);
    return false;
  }
}

ImageType Image::getFileType(const char* filename) {
	const char* ext = strrchr(filename, '.');
	if(ext != nullptr) {
		if(strcmp(ext, ".png") == 0) {
			return PNG;
		}
		else if(strcmp(ext, ".jpg") == 0) {
			return JPG;
		}
		else if(strcmp(ext, ".bmp") == 0) {
			return BMP;
		}
		else if(strcmp(ext, ".tga") == 0) {
			return TGA;
		}
	}
	return PNG;
}




Image& Image::overlay(const Image& source, int x, int y) {

	uint8_t* srcPx;
	uint8_t* dstPx;

	for(int sy = 0;sy < source.h;sy++) {

		if(sy + y < 0) {continue;}
		else if(sy + y >= h) {break;}

		for(int sx = 0;sx < source.w;sx++) {

			if(sx + x < 0) {continue;}
			else if(sx + x >= w) {break;}
			
			srcPx = &source.data[(sx + sy * source.w) * source.channels];
			dstPx = &data[(sx + x + (sy + y) * w) * channels];

			float srcAlpha = source.channels < 4 ? 1 : srcPx[3] / 255.f;
			float dstAlpha = channels < 4 ? 1 : 1;//dstPx[3] / 255.f;
			//std::cout << srcAlpha << "|" << dstAlpha << std::endl;
			if(srcAlpha > .99 && dstAlpha > .99) {
				if(source.channels >= channels) {
					memcpy(dstPx, srcPx, channels*sizeof(uint8_t));
				}
				else {
					// In case our source image is grayscale and the dest one isnt
					memset(dstPx, srcPx[0], channels*sizeof(uint8_t));
				}
			}
			
			else {
				float outAlpha = srcAlpha + dstAlpha * (1 - srcAlpha);
				if(outAlpha < .01) {
					memset(dstPx, 0, channels*sizeof(uint8_t));
				}
				else {
					for(int chnl = 0;chnl < channels;chnl++) {
						dstPx[chnl] = (uint8_t)BYTE_BOUND((srcPx[chnl]/255.f * srcAlpha + dstPx[chnl]/255.f * dstAlpha * (1 - srcAlpha)) / outAlpha * 255.f);
						//dstPx[chnl] = ((float)srcPx[chnl]/255.f * srcAlpha + (float)dstPx[chnl]/255.f * dstAlpha * (1.f - srcAlpha)) / outAlpha * 255.f;
					}
					if(channels > 3) {dstPx[3] = (int)BYTE_BOUND(outAlpha * 255.f);}
				}
			}
			
		}

	}
	return *this;
}


Image& Image::resize(uint16_t x_new, uint16_t y_new){
	uint8_t* dstPx = new uint8_t[x_new*y_new*channels];
	memset(dstPx,0,x_new*y_new*channels);
	float x_scale = x_new/float(h-1);
	//cout<< x_scale << endl; 
	float y_scale = y_new/float(w-1);
	//cout<< y_scale <<endl; 
	int tmpX, tmpY;
	int compt = 0; 
	for(uint16_t sy = 0; sy < y_new; sy++){
		for(uint16_t sx = 0; sx < x_new; sx++){
			tmpY = round(sy/x_scale)+1;  
			tmpX = round(sx/y_scale)+1;
			//cout << tmpX<<endl; 
			memcpy(&dstPx[(sx + sy*x_new)*channels], &data[(tmpY*w + tmpX)*channels], channels);
		}
	}
	w = x_new; 
	h = y_new; 
	size = w*h*channels;
	
	delete[] data; 
	data = dstPx;
	dstPx = nullptr;
	return *this; 
}