#include <stdio.h>
#include <iostream>
#include "faceswap_func.h"
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;
//#include "homographie.h"

void hello() {
	printf("Hello World !\n");
}

void addition(int a, int b) {
	int res;
	res = a + b;
	printf("addition : %d + %d = %d\n", a, b, res);
}

void fillTab(py::list tab , int size){
    for(int i = 0; i < size; i++){
        tab[i] = i;
    }
}

void loadImage(py::array_t<int> img, int width, int height) {
    py::buffer_info imgBuff = img.request();
    int* imgPtr = (int*)imgBuff.ptr;

    for(size_t y = 0; y < height ; y++){
        for(size_t x = 0; x < width; x++){
            printf("| %d/%d/%d ", imgPtr[y*width + x*3 + 0], imgPtr[y*width + x*3 + 1], imgPtr[y*width + x*3 + 2]);
        }
        printf("\n");
    }
}
// Define functions here


/*void FindAllHomography(int n_quadrangles, int** Quadrangles, int** landmarks_src, int** landmarks_dst, double ** H){
    int * Q_coords_src, Q_coords_dst;
    Q_coords_src = (int*)calloc(8, sizeof(int));
    Q_coords_dst = (int*)calloc(8, sizeof(int));

    for(int i = 0; i < n_quadrangles; i++){
        for(int j = 0; j < 4; j++){
            //Q_coords = [ x0, y0, x1, y1, x2, y2, x3, y3]
            Q_coords_src[2*j]   = landmarks_src[Quadrangles[i][j]][0];
            Q_coords_src[2*j+1] = landmarks_src[Quadrangles[i][j]][1];

            Q_coords_dst[2*j]   = landmarks_dst[Quadrangles[i][j]][0];
            Q_coords_dst[2*j+1] = landmarks_dst[Quadrangles[i][j]][1];
        }
        Find_Homography(Q_coords_src, Q_coords_dst, H[i]);
    }
}  

void Find_rectangle(double *src,double *rect)
{
    int minx,maxx,miny,maxy;
    minx=maxx=src[0];
    miny=maxy=src[1];
    for(int i=0; i<3;i++){
        if (src[i*2]<minx)
            minx=src[i*2];
        if (src[i*2]>maxx)
            maxx=src[i*2];
        if (src[i*2+1]<miny)
            miny=src[i*2+1];
        if (src[i*2+1]=maxy)
            maxy=src[i*2+1];    
    }
    rect[0]=minx;rect[1]=miny;rect[2]=maxx-minx;rect[3]=maxy-miny;

}
//TO FINISH
void ApplyAllHomography(unsigned char * imgSrc, unsigned char * imgDst, int width, int height, int * imgLabel, int ** H, double* XI, double* YI){
    double* p;
    p = (double*)calloc(2, sizeof(double));
    for(int y = 0; y < height; y++){
        for(int x = 0; x < width; x++){
            if (imgLabel[y*width+x] > -1){
                ApplyPointHomography(H[imgLabel[y*width+x]], [(double)x, (double)y], p);
                imgDst[y*width+x] = imgSrc[p[1]]
                XI[y*width+x] = p[0];
                YI[y*width+x] = p[1];
            }else{
                XI[y*width+x] = x;
                YI[y*width+x] = y;
            }
        }
    }
}

void FaceSwap(){
    //1) Determine all homographies

    //2) Apply homographies and create interpolation grids

    //3) Recreate new image
}
void CreateLabelledImage(int n_quadrangles, int** Quadrangles, int** landmarks, int heigth, int width, double* imgLabel) {
	//init labelled image
    for(int i = 0; i < width*height; i++){
        imgLabel[i] = -1; 
    }

    double Sqr_coords[8] = {-1, 1, 1, 1, 1, -1, -1, -1};
	double* Q_coords, Q_box, H, m, p; 
    Q_coords = (double*)calloc(8, sizeof(double));
    Q_box    = (double*)calloc(8, sizeof(double));
    H        = (double*)calloc(9, sizeof(double));
    m        = (double*)calloc(2, sizeof(double));
    p        = (double*)calloc(2, sizeof(double));

    for(int i = 0; i < n_quadrangles; i++){
        for(int j = 0; j < 4; j++){
            //Q_coords = [ x0, y0, x1, y1, x2, y2, x3, y3]
            Q_coords[i][2*j]   = landmarks[Quadrangles[i][j]][0];
            Q_coords[i][2*j+1] = landmarks[Quadrangles[i][j]][1];
        }
        Find_Homography(Q_coords, Sqr_coords, H);
        Find_rectangle(Q_coords, Q_box);
        //Q_box[0] = x0 / Q_box[1] = y0 / Q_box[2] = width / Q_box[3] = height
        for(int y = Q_box[1]; y < Q_box[1]+Q_box[3]; y++){
            for(int x = Q_box[0]; x < Q_box[0]+Q_box[2]; x++){
                m[0] = x; m[1] = y;
                applique_homo(H, m, p);
                if( abs(p[0]) < 1  && abs(p[1]) < 1){
                    imgLabel[(y+y0)*width+(x+x0)] = i;
                }
            }
        }
    }
}*/