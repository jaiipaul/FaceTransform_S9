#ifndef _UTILS_H_
#define _UTILS_H_


void Find_rectangle(float* src,float* rect);

void Flatten(float** Tab2D, int w, int h, float* Tab1D);

void CreateLabelledImage(int n_quadrangles, int* Quadrangles, 
                         int* landmarks, int width, int heigth, 
                         int* imgLabel);

void RecreateImage_1(int* imgOut, 
                     int* imgIn ,int width, int height, 
                     float* XI, float* YI, int* imgLabel, int mode); 

void RecreateImage_2(int* imgOut, 
                     int* img1 ,int width_1, int height_1, 
                     int* img2, int width_2, int height_2, 
                     float* XI, float* YI, int* imgLabel, int mode);    
                                
#endif //_UTILS_H_