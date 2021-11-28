#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "faceswap_func.h"
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "homographie.h"
namespace py = pybind11;


//Test functions for wrapper
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


void FindAllHomography(int n_quadrangles, int* Quadrangles, int* landmarks_src, int* landmarks_dst, double ** H){
    printf("Find homographies\n");
    double * Q_coords_src, * Q_coords_dst;
    Q_coords_src = (double*)calloc(8, sizeof(double));
    Q_coords_dst = (double*)calloc(8, sizeof(double));

    for(int i = 0; i < n_quadrangles; i++){
        for(int j = 0; j < 4; j++){
            //Q_coords = [ x0, y0, x1, y1, x2, y2, x3, y3]
            Q_coords_src[2*j]   = landmarks_src[Quadrangles[i*4+j]];
            Q_coords_src[2*j+1] = landmarks_src[Quadrangles[i*4+j]+1];

            Q_coords_dst[2*j]   = landmarks_dst[Quadrangles[i*4+j]];
            Q_coords_dst[2*j+1] = landmarks_dst[Quadrangles[i*4+j]+1];
        }
        Find_Homography(Q_coords_src, Q_coords_dst, H[i]);
    }
    free(Q_coords_dst);
    free(Q_coords_src);
}  

void Find_rectangle(double *src,double *rect)
{
    double minx,maxx,miny,maxy;
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
void ApplyAllHomography(int* imgSrc, int* imgDst, int width, int height, int* imgLabel, double ** H, double* XI, double* YI){
    printf("Apply homographies\n");
    double* m, *p;
    m = (double*)calloc(2, sizeof(double));
    p = (double*)calloc(2, sizeof(double));
    for(int y = 0; y < height; y++){
        for(int x = 0; x < width; x++){
            if (imgLabel[y*width+x] > 0){
                m[0] = x; m[1] = y;
                ApplyPointHomography(H[(int)imgLabel[y*width+x]-1], m, p);
                
                XI[y*width+x] = p[0];
                YI[y*width+x] = p[1];
            }else{
                XI[y*width+x] = x;
                YI[y*width+x] = y;
            }
        }
    }
    free(m);
    free(p);
}

void CreateLabelledImage(int n_quadrangles, int* Quadrangles, int* landmarks, int width, int height, int* imgLabel) {
	//init labelled image
    printf("Create label image\n");
    for(int i = 0; i < width*height; i++){
        imgLabel[i] = 0; 
    }

    double* Sqr_coords = (double*)calloc(8, sizeof(double));
    Sqr_coords[0] = -1.0;
    Sqr_coords[1] =  1.0;
    Sqr_coords[2] =  1.0;
    Sqr_coords[3] =  1.0;
    Sqr_coords[4] =  1.0;
    Sqr_coords[5] = -1.0;
    Sqr_coords[6] = -1.0;
    Sqr_coords[7] = -1.0;

    printMat(Sqr_coords, 8, 1);
    //printf("Sqr_coords_ptr = [ %f  %f  %f  %f  %f  %f  %f  %f]\n", Sqr_coords_ptr[0], Sqr_coords_ptr[1], Sqr_coords_ptr[2], Sqr_coords_ptr[3], Sqr_coords_ptr[4], Sqr_coords_ptr[5], Sqr_coords_ptr[6], Sqr_coords_ptr[7]);
    double *Q_coords, *Q_box, *H, *m, *p;
    Q_coords = (double*)calloc(8, sizeof(double));
    Q_box    = (double*)calloc(4, sizeof(double));
    H        = (double*)calloc(9, sizeof(double));
    m        = (double*)calloc(2, sizeof(double));
    p        = (double*)calloc(2, sizeof(double));

    for(int i = 0; i < n_quadrangles; i++){
        for(int j = 0; j < 4; j++){
            //Q_coords = [ x0, y0, x1, y1, x2, y2, x3, y3]
            Q_coords[2*j]   = (double)landmarks[Quadrangles[i*4 + j]];
            Q_coords[2*j+1] = (double)landmarks[Quadrangles[i*4 + j]+1];
        }
        //printf("Q_coords[%d] = [ %f  %f  %f  %f  %f  %f  %f  %f]\n", i, Q_coords[0], Q_coords[1], Q_coords[2], Q_coords[3], Q_coords[4], Q_coords[5], Q_coords[6], Q_coords[7]);
        Find_Homography(Q_coords, Sqr_coords, H);
        //printf("H[%d] = [ %f  %f  %f ]\n        [ %f  %f  %f ]\n        [ %f  %f  %f ]\n", i, H[0], H[3], H[6], H[1], H[4], H[7], H[2], H[5], H[8]);
        Find_rectangle(Q_coords, Q_box);
        //Q_box[0] = x0 / Q_box[1] = y0 / Q_box[2] = width / Q_box[3] = height
        for(int y = Q_box[1]; y < Q_box[1]+Q_box[3]; y++){
            for(int x = Q_box[0]; x < Q_box[0]+Q_box[2]; x++){
                m[0] = x; m[1] = y;
                //printf("%f // %f\n", m[0], m[1]);
                ApplyPointHomography(H, m, p);
                //printf("%f // %f\n", p[0], p[1]);
                if( abs(p[0]) < 1.0  && abs(p[1]) < 1.0){
                    
                    imgLabel[(y+(int)Q_box[1])*width+(x+(int)Q_box[0])] = i+1;
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

py::array_t<int> FaceSwap(py::array_t<int> img_CAM, py::array_t<int> img_FTA,
              int width_CAM,int height_CAM, int width_FTA, int height_FTA,
              int n_quadrangles, py::array_t<int> Quadrangles, 
              py::array_t<int> landmarks_CAM, py::array_t<int> landmarks_FTA){
    
    //Define pointers for images
    py::buffer_info imgFTA_Buff = img_FTA.request();
    py::buffer_info imgCAM_Buff = img_CAM.request();

    int* imgFTA_Ptr = (int*)imgFTA_Buff.ptr;
    int* imgCAM_Ptr = (int*)imgCAM_Buff.ptr;

    //Define pointers for landmark points tab
    //Landmarks = [x0, y0, x1, y1, ... , xn, yn]
    py::buffer_info landmarksFTA_Buff = landmarks_FTA.request();
    py::buffer_info landmarksCAM_Buff = landmarks_CAM.request();
    int* landmarksFTA_Ptr = (int*)landmarksFTA_Buff.ptr;
    int* landmarksCAM_Ptr = (int*)landmarksCAM_Buff.ptr;

    //Define pointer for quadrangle tab
    //Quadrangles = [idx01, idx02, idx03, idx04, idx11, idx12, idx13, idx14, ... , idxn1, idxn2, idxn3, idxn4] 
    py::buffer_info Quadrangles_Buff = Quadrangles.request();
    int* Quadrangles_Ptr = (int*)Quadrangles_Buff.ptr;

    //1) Determine all homographies
    double** H;
    H = (double**)calloc(n_quadrangles, sizeof(double*));
    H[0] = (double*)calloc(9*n_quadrangles, sizeof(double));
    for(int i = 0; i < n_quadrangles; i++){
        H[i] = H[0] + (i*9);
    }
    //FindAllHomography(n_quadrangles, Quadrangles_Ptr, landmarksFTA_Ptr, landmarksCAM_Ptr, H);
    //2) Create Labelled image to know wich homographt to apply
    int* imgLabel;
    imgLabel = (int*)calloc(width_CAM*height_CAM, sizeof(int));
    CreateLabelledImage(n_quadrangles, Quadrangles_Ptr, landmarksCAM_Ptr, width_CAM, height_CAM, imgLabel);

    /*
    //3) Apply homographies and create interpolation grids
    double *XI, *YI;
    XI = (double*)calloc((double)width_CAM*height_CAM, sizeof(double));
    YI = (double*)calloc((double)width_CAM*height_CAM, sizeof(double));
    ApplyAllHomography(imgFTA_Ptr, imgCAM_Ptr, width_CAM, height_CAM, imgLabel, H, XI, YI);
    */
    //4) Recreate new image
    auto imgOut = py::array_t<int>(width_CAM*height_CAM);
    py::buffer_info imgOut_Buff = imgOut.request();
    int* imgOut_Ptr = (int*)(imgOut_Buff.ptr);
    /*
    for(int y = 0; y < 480; y++){
        for(int x = 0; x < 640; x++){
            if(imgLabel[y*width_CAM+x] > -1){ 
                
                imgOut_Ptr[y * width_CAM + x * 3] = imgCAM_Ptr[y * width_CAM + x * 3];
                imgOut_Ptr[y * width_CAM + x * 3 + 1] = imgCAM_Ptr[y * width_CAM + x * 3 + 1];
                imgOut_Ptr[y * width_CAM + x * 3 + 2] = imgCAM_Ptr[y * width_CAM + x * 3 + 2];
                printf("x : %d / y : %d \n", x, y);  
                //imgOut_Ptr[y * width_CAM + x * 3] = 1;// imgFTA_Ptr[(int)round(YI[y * width_CAM + x]) * width_FTA + (int)round(XI[y * width_CAM + x]) * 3];
                //imgOut_Ptr[y * width_CAM + x * 3 + 1] = 2;// imgFTA_Ptr[(int)round(YI[y * width_CAM + x]) * width_FTA + (int)round(XI[y * width_CAM + x]) * 3 + 1];
                //imgOut_Ptr[y * width_CAM + x * 3 + 2] = 3;// imgFTA_Ptr[(int)round(YI[y * width_CAM + x]) * width_FTA + (int)round(XI[y * width_CAM + x]) * 3 + 2];
            }else{
                imgOut_Ptr[y * width_CAM + x * 3]     = imgCAM_Ptr[y * width_CAM + x*3];
                imgOut_Ptr[y * width_CAM + x * 3 + 1] = imgCAM_Ptr[y * width_CAM + x*3 + 1];
                imgOut_Ptr[y * width_CAM + x * 3 + 2] = imgCAM_Ptr[y * width_CAM + x*3 + 2];
                //printf("x : %d / y : %d \n", x, y);  
            }
        }
    }*/
    for (int i = 0; i < width_CAM * height_CAM; i++) {
        imgOut_Ptr[i] = imgLabel[i];
    }
    
    printf("CPP done");

    //Liberation mémoire
    
    free(H[0]);
    free(H);
    //free(XI);
    //free(YI);
    free(imgLabel);

    return(imgOut);
}


