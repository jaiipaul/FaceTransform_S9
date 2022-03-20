#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <opencv2/opencv.hpp>
#include <iostream>
#include "homographie.h"

void Find_Homography(float* src, float* dst, float *h){
    std::vector<cv::Point2f> Src;
    std::vector<cv::Point2f> Dst;
    for(int i = 0; i < 4; i++ ){
        Src.push_back(cv::Point2f(src[2*i],src[2*i+1]));
        Dst.push_back(cv::Point2f(dst[2*i],dst[2*i+1]));
    }

    cv::Mat H = (cv::Mat_<double>(3,3));
    H = cv::findHomography(Dst, Src);
    
    for( int i = 0; i < 3; i++){
        for (int j = 0; j < 3; j++){
            h[i*3 + j] = (float)H.at<double>(j,i);
        }
    }
}

void FindAllHomography(int n_quadrangles, int* Quadrangles, int* landmarks_src, int* landmarks_dst, float ** H){
    float * Q_coords_src, * Q_coords_dst;
    Q_coords_src = (float*)calloc(8, sizeof(float));
    Q_coords_dst = (float*)calloc(8, sizeof(float));

    for(int i = 0; i < n_quadrangles; i++){
        for(int j = 0; j < 4; j++){
            //Q_coords = [ x0, y0, x1, y1, x2, y2, x3, y3]
            Q_coords_src[2*j]   = (float)landmarks_src[2*Quadrangles[i*4+j]];
            Q_coords_src[2*j+1] = (float)landmarks_src[2*Quadrangles[i*4+j]+1];

            Q_coords_dst[2*j]   = (float)landmarks_dst[2*Quadrangles[i*4+j]];
            Q_coords_dst[2*j+1] = (float)landmarks_dst[2*Quadrangles[i*4+j]+1];
        }
        Find_Homography(Q_coords_src, Q_coords_dst, H[i]);
    }
    free(Q_coords_dst);
    free(Q_coords_src);
}

void ApplyPointHomography(float *h, float *m, float *p)
{
    float den = 1.0f / (h[2]*m[0]+h[5]*m[1]+h[8]);
    p[0]=(h[0]*m[0]+h[3]*m[1]+h[6])*den;
    p[1]=(h[1]*m[0]+h[4]*m[1]+h[7])*den;
}

void ApplyAllHomography(int width, int height, int* imgLabel, float** H, float* XI, float* YI){
    
    float* m, *p;
    m = (float*)calloc(2, sizeof(float));
    p = (float*)calloc(2, sizeof(float));
    for(int y = 0; y < height; y++){
        for(int x = 0; x < width; x++){
            if (imgLabel[y*width+x] > 0){
                m[0] = x; m[1] = y;
                ApplyPointHomography(H[imgLabel[y*width+x]-1], m, p);
                XI[y*width+x] = p[0];
                YI[y*width+x] = p[1];

            }else{
                XI[y*width+x] = 0;
                YI[y*width+x] = 0;
            }
        }
    }
    free(m);
    free(p);
}