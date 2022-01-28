#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "faceswap_func.h"
#include "homographie.h"
#include "utils.h"
#include "cuda_func.cuh"

namespace py = pybind11;
// Define functions here

py::array_t<int> FaceSwap_CPP(py::array_t<int> img_CAM, py::array_t<int> img_FTA,
                              int width_CAM,int height_CAM, int width_FTA, int height_FTA,
                              int n_quadrangles, py::array_t<int> Quadrangles, 
                              py::array_t<int> landmarks_CAM, py::array_t<int> landmarks_FTA){
    clock_t time;
    time = clock();

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
    float** H = NULL;
    H = (float**)calloc(n_quadrangles, sizeof(float*));
    H[0] = (float*)calloc(9*n_quadrangles, sizeof(float));
    for(int i = 0; i < n_quadrangles; i++){
        H[i] = H[0] + (i*9);
    }
        
    FindAllHomography(n_quadrangles, Quadrangles_Ptr, landmarksFTA_Ptr, landmarksCAM_Ptr, H);
    //3) Apply homographies and create interpolation grids
    float *XI = NULL, *YI = NULL;
    XI = (float*)calloc(width_CAM*height_CAM, sizeof(float));
    YI = (float*)calloc(width_CAM*height_CAM, sizeof(float));

    //for(int i = 0; i < 100;  i++){
    ApplyAllHomography(width_CAM, height_CAM, imgLabel, H, XI, YI);
    //}
    
    //4) Recreate new image
    auto imgOut = py::array_t<int>(width_CAM*height_CAM*3);
    py::buffer_info imgOut_Buff = imgOut.request();
    int* imgOut_Ptr = (int*)(imgOut_Buff.ptr);
    
    RecreateImage_2(imgOut_Ptr, 
                     imgCAM_Ptr, width_CAM, height_CAM, 
                     imgFTA_Ptr, width_FTA, height_FTA, 
                     XI, YI, imgLabel, 2);


    //Liberation mémoire
    
    free(H[0]);
    free(H);
    free(XI);
    free(YI);
    free(imgLabel);

    time = clock() - time;

    //std::cout << "Swapping faces took : " << ((float)time)/CLOCKS_PER_SEC << " seconds with C++" << std::endl;

    return(imgOut);
}


py::array_t<int> FaceSwap_CUDA(py::array_t<int> img_CAM, py::array_t<int> img_FTA,
                               int width_CAM,int height_CAM, int width_FTA, int height_FTA,
                               int n_quadrangles, py::array_t<int> Quadrangles, 
                               py::array_t<int> landmarks_CAM, py::array_t<int> landmarks_FTA){
    
    clock_t time;
    time = clock();
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
    float** H = NULL;
    H = (float**)calloc(n_quadrangles, sizeof(float*));
    H[0] = (float*)calloc(9*n_quadrangles, sizeof(float));
    for(int i = 0; i < n_quadrangles; i++){
        H[i] = H[0] + (i*9);
    }
        
    FindAllHomography(n_quadrangles, Quadrangles_Ptr, landmarksFTA_Ptr, landmarksCAM_Ptr, H);
    //3) Apply homographies and create interpolation grids
    float *XI = NULL, *YI = NULL;
    float *H_flat = NULL;
    XI = (float*)calloc(width_CAM*height_CAM, sizeof(float));
    YI = (float*)calloc(width_CAM*height_CAM, sizeof(float));

    H_flat = (float*)calloc(9*n_quadrangles, sizeof(float));
    Flatten(H, 9, n_quadrangles, H_flat);

    float kernel_time = 0.f;
    kernel_time = ApplyAllHomography_CUDA(width_CAM, height_CAM, imgLabel, H_flat, XI, YI, n_quadrangles);
    
    //4) Recreate new image
    auto imgOut = py::array_t<int>(width_CAM*height_CAM*3);
    py::buffer_info imgOut_Buff = imgOut.request();
    int* imgOut_Ptr = (int*)(imgOut_Buff.ptr);
    
    
    RecreateImage_2(imgOut_Ptr, 
                  imgCAM_Ptr, width_CAM, height_CAM, 
                  imgFTA_Ptr, width_FTA, height_FTA, 
                  XI, YI, imgLabel, 2);


    //Liberation mémoire
    
    free(H[0]);
    free(H);
    free(H_flat);
    free(XI);
    free(YI);
    free(imgLabel);

    time = clock() - time;

    //std::cout << "Swapping faces took : " << ((float)time)/CLOCKS_PER_SEC << " seconds with CUDA" << std::endl;
    return(imgOut);
}
