#include "utils.h"
#include "homographie.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>

void Find_rectangle(float *src, float *rect){
    
    float minx,maxx,miny,maxy;
    minx=maxx=src[0];
    miny=maxy=src[1];
    for(int i=1; i<4;i++){
        if (src[i*2]<minx)
            minx=src[i*2];
        else if (src[i*2]>maxx)
            maxx=src[i*2];
        if (src[i*2+1]<miny)
            miny=src[i*2+1];
        else if (src[i*2+1]>maxy)
            maxy=src[i*2+1];    
    }
    rect[0]=minx;rect[1]=miny;
    rect[2]=maxx;rect[3]=miny;
    rect[4]=maxx;rect[5]=maxy;
    rect[6]=minx;rect[7]=maxy;

}

void Flatten(float** Tab2D, int w, int h, float* Tab1D){
    for(int j = 0; j < h; j++){
        for(int i = 0; i < w; i++){
            Tab1D[j*w+i] = Tab2D[j][i];
        }
    }
}


void CreateLabelledImage(int n_quadrangles, int* Quadrangles, int* landmarks, int width, int height, int* imgLabel) {

    for(int i = 0; i < width*height; i++){
        imgLabel[i] = 0; 
    }

    float* Sqr_coords = (float*)calloc(8, sizeof(float));
    Sqr_coords[0] =  -100.0f;
    Sqr_coords[1] =  -100.0f;
    Sqr_coords[2] =  100.0f;
    Sqr_coords[3] =  -100.0f;
    Sqr_coords[4] =  100.0f;
    Sqr_coords[5] =  100.0f;
    Sqr_coords[6] =  -100.0f;
    Sqr_coords[7] =  100.0f;

    float *Q_coords, *Q_box, *H, *m, *p;
    Q_coords = (float*)calloc(8, sizeof(float));
    Q_box    = (float*)calloc(8, sizeof(float));
    H        = (float*)calloc(9, sizeof(float));
    m        = (float*)calloc(2, sizeof(float));
    p        = (float*)calloc(2, sizeof(float));

    for(int i = 0; i < n_quadrangles; i++){
        for(int j = 0; j < 4; j++){
            //Q_coords = [ x0, y0, x1, y1, x2, y2, x3, y3]
            Q_coords[2*j]   = (float)landmarks[2*Quadrangles[i*4 + j]];
            Q_coords[2*j+1] = (float)landmarks[2*Quadrangles[i*4 + j]+1];
        }
        
        Find_rectangle(Q_coords, Q_box);
        //Q_box = [ x0, y0, x1, y1, x2, y2, x3, y3]
        Find_Homography(Sqr_coords, Q_coords, H);  
        for(int y = Q_box[1]; y < Q_box[5]+1; y++){
            for(int x = Q_box[0]; x < Q_box[2]+1; x++){
                m[0] = (float)x; m[1] = (float)y;
                ApplyPointHomography(H, m, p);

                if(abs(p[0]) <= 100.0 && abs(p[1]) <= 100.0){ 
                    imgLabel[y*width+x] = i+1;
                }
            }
        }
    }
    
    free(H);
    free(Sqr_coords);
    free(Q_coords);
    free(Q_box);
    free(m);
    free(p);
}

void RecreateImage_1(int* imgOut, 
                     int* imgIn ,int width, int height, 
                     float* XI, float* YI, int* imgLabel, int mode){ 
    
    int colors[53][3];
    if(mode == 3){
        for(int i = 0; i < 53; i++){
            colors[i][0] = rand()%255;
            colors[i][1] = rand()%255;
            colors[i][2] = rand()%255;
        }
    }

    for(int y = 0; y < height; y++){
        for(int x = 0; x < width; x++){
            int label = imgLabel[y*width+x];
            if(label > 0){ 
                int x_2 = floor(XI[y * width + x]);
                int y_2 = floor(YI[y * width + x]);

                switch(mode){
                    case 1 : /*Closest Neighboor*/
                        if(x_2 >= 0 && x_2 < width && y_2 >= 0 && y_2 < height){
                            for(int c = 0; c < 3; c++){
                                imgOut[y * 3*width + x * 3 + c] = imgIn[(int)y_2 * 3*width + (int)x_2 * 3 + c];
                            }
                        }
                        break;

                    case 2 : /*Bilinear*/
                        if(x_2 >= 0 && x_2 < width && y_2 >= 0 && y_2 < height){
                            float dx_2 = fmod(XI[y * width + x],x_2);
                            float dy_2 = fmod(YI[y * width + x],y_2);

                            for(int c = 0; c < 3; c++){
                                int pixel_value =   (1.f-dx_2)*(1.f-dy_2)*imgIn[y_2 * 3*width + x_2 * 3 + c]
                                                  + dx_2*(1.f-dy_2)*imgIn[y_2 * 3*width + (x_2 + 1) * 3 + c]
                                                  + dy_2*(1.f-dx_2)*imgIn[(y_2 + 1) * 3*width + x_2 * 3 + c]
                                                  + dy_2*dx_2*imgIn[(y_2 + 1) * 3*width + (x_2 + 1) * 3 + c];

                                imgOut[y * 3*width + x * 3 + c] = pixel_value;
                            }
                        }
                        break;

                    case 3 : /* Fill with color*/
                        for(int c = 0; c < 3; c++){
                            imgOut[y * 3*width + x * 3 + c] = colors[label-1][c];
                        }
                        break;

                    default:
                        break;      
                }
            }else{
                imgOut[y * 3*width + x * 3]     = imgIn[y * 3*width + x*3];
                imgOut[y * 3*width + x * 3 + 1] = imgIn[y * 3*width + x*3 + 1];
                imgOut[y * 3*width + x * 3 + 2] = imgIn[y * 3*width + x*3 + 2];
            }
        }  
    }
}

void RecreateImage_2(int* imgOut, 
                     int* img1 ,int width_1, int height_1, 
                     int* img2, int width_2, int height_2, 
                     float* XI, float* YI, int* imgLabel, int mode){ 
    
    int colors[53][3];
    if(mode == 3){
        for(int i = 0; i < 53; i++){
            colors[i][0] = rand()%255;
            colors[i][1] = rand()%255;
            colors[i][2] = rand()%255;
        }
    }

    for(int y = 0; y < height_1; y++){
        for(int x = 0; x < width_1; x++){
            int label = imgLabel[y*width_1+x];
            if(label > 0){ 
                int x_2 = floor(XI[y * width_1 + x]);
                int y_2 = floor(YI[y * width_1 + x]);

                switch(mode){
                    case 1 : /*Closest Neighboor*/
                        if(x_2 >= 0 && x_2 < width_2 && y_2 >= 0 && y_2 < height_2){
                            for(int c = 0; c < 3; c++){
                                imgOut[y * 3*width_1 + x * 3 + c] = img2[(int)y_2 * 3*width_2 + (int)x_2 * 3 + c];
                            }
                        }
                        break;

                    case 2 : /*Bilinear*/
                        if(x_2 >= 0 && x_2 < width_2 && y_2 >= 0 && y_2 < height_2){
                            float dx_2 = fmod(XI[y * width_1 + x],x_2);
                            float dy_2 = fmod(YI[y * width_1 + x],y_2);

                            for(int c = 0; c < 3; c++){
                                int pixel_value =   (1.f-dx_2)*(1.f-dy_2)*img2[y_2 * 3*width_2 + x_2 * 3 + c]
                                                  + dx_2*(1.f-dy_2)*img2[y_2 * 3*width_2 + (x_2 + 1) * 3 + c]
                                                  + dy_2*(1.f-dx_2)*img2[(y_2 + 1) * 3*width_2 + x_2 * 3 + c]
                                                  + dy_2*dx_2*img2[(y_2 + 1) * 3*width_2 + (x_2 + 1) * 3 + c];

                                imgOut[y * 3*width_1 + x * 3 + c] = pixel_value;
                            }
                        }
                        break;

                    case 3 : /* Fill with color*/
                        for(int c = 0; c < 3; c++){
                            imgOut[y * 3*width_1 + x * 3 + c] = colors[label-1][c];
                        }
                        break;

                    default:
                        break;      
                }
            }else{
                imgOut[y * 3*width_1 + x * 3]     = img1[y * 3*width_1 + x*3];
                imgOut[y * 3*width_1 + x * 3 + 1] = img1[y * 3*width_1 + x*3 + 1];
                imgOut[y * 3*width_1 + x * 3 + 2] = img1[y * 3*width_1 + x*3 + 2];
            }
        }  
    }
}