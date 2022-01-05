#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>

#include "faceswap_func.h"
#include "faceswap_func_cuda.h"

#include "homographie.h"
#include "pybind_cuda.cuh"

py::array_t<int> CUDA_Sqr(py::array_t<int> img, int width, int height) {
    
    py::buffer_info imgBuff = img.request();
    int* imgPtr = (int*)imgBuff.ptr;

    auto imgOut = py::array_t<int>(width*height);
    py::buffer_info imgOut_Buff = imgOut.request();
    int* imgOut_Ptr = (int*)(imgOut_Buff.ptr);

    gpu_square_main(imgPtr, width*height, imgOut_Ptr);
    //std::cout << "CUDA done" << std::endl;
    return(imgOut);
}

namespace py = pybind11;
// Define functions here
 
void Flatten(double** Tab2D, int w, int h, double* Tab1D){
    for(int j = 0; j < h; j++){
        for(int i = 0; i < w; i++){
            Tab1D[j*w+i] = Tab2D[j][i];
        }
    }
}

py::array_t<int> FaceSwap_CUDA(py::array_t<int> img_CAM, py::array_t<int> img_FTA,
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

    //printMat(landmarksCAM_Ptr, 136, 1);
    //Define pointer for quadrangle tab
    //Quadrangles = [idx01, idx02, idx03, idx04, idx11, idx12, idx13, idx14, ... , idxn1, idxn2, idxn3, idxn4] 
    py::buffer_info Quadrangles_Buff = Quadrangles.request();
    int* Quadrangles_Ptr = (int*)Quadrangles_Buff.ptr;

    
    //1) Create Labelled image to know wich homographt to apply
    int* imgLabel =NULL;
    imgLabel = (int*)calloc(width_CAM*height_CAM, sizeof(int));
    CreateLabelledImage(n_quadrangles, Quadrangles_Ptr, landmarksCAM_Ptr, width_CAM, height_CAM, imgLabel);

    //2) Determine all homographies
    double** H = NULL;
    H = (double**)calloc(n_quadrangles, sizeof(double*));
    H[0] = (double*)calloc(9*n_quadrangles, sizeof(double));
    for(int i = 0; i < n_quadrangles; i++){
        H[i] = H[0] + (i*9);
    }
        
    FindAllHomography(n_quadrangles, Quadrangles_Ptr, landmarksFTA_Ptr, landmarksCAM_Ptr, H);
    //3) Apply homographies and create interpolation grids
    int *XI = NULL, *YI = NULL;
    double *H_flat = NULL;
    XI = (int*)calloc(width_CAM*height_CAM, sizeof(int));
    YI = (int*)calloc(width_CAM*height_CAM, sizeof(int));

    H_flat = (double*)calloc(9*n_quadrangles, sizeof(double));
    Flatten(H, 9, n_quadrangles, H_flat);
    printf("Try to apply all homographies\n");
    ApplyAllHomography_CUDA(width_CAM, height_CAM, imgLabel, H_flat, XI, YI, n_quadrangles);
    printf("Applied all homographies\n");
    //4) Recreate new image
    auto imgOut = py::array_t<int>(width_CAM*height_CAM*3);
    py::buffer_info imgOut_Buff = imgOut.request();
    int* imgOut_Ptr = (int*)(imgOut_Buff.ptr);
    
    
    printf("Try to Recreate image\n");
    RecreateImage_CUDA(imgOut_Ptr, 
                       imgCAM_Ptr, width_CAM, height_CAM, 
                       imgFTA_Ptr, width_FTA, height_FTA, 
                       XI, YI, imgLabel);
    printf("Recreated Image\n");
    /*for (int i = 0; i < width_CAM * height_CAM; i++) {
        imgOut_Ptr[i] = imgLabel[i];
    }*/
    //printf("CPP done");

    //Liberation mémoire
    
    free(H[0]);
    free(H);
    free(H_flat);
    free(XI);
    free(YI);
    free(imgLabel);
    //free(imgOut_Buff.ptr);
    //free(imgCAM_Buff.ptr);
    //free(imgFTA_Buff.ptr);
    //free(landmarksCAM_Buff.ptr);
    //free(landmarksFTA_Buff.ptr);
    //free(Quadrangles_Buff.ptr);

    return(imgOut);
}


