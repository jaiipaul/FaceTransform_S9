#ifndef _FACESWAP_FUNC_H_
#define _FACESWAP_FUNC_H_

#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

namespace py = pybind11;

// Define function here 

void FS_FindAllHomography(int n_quadrangles, int* Quadrangles, int* landmarks_src, int* landmarks_dst, float** H);

void FS_Find_rectangle(float* src,float* rect);
//TO FINISH
void FS_ApplyAllHomography(int width, int height, int* imgLabel, float** H, float* XI, float* YI);

void FS_RecreateImage(int* imgOut, 
                   int* imgCAM ,int width_CAM, int height_CAM, 
                   int* imgFTA, int width_FTA, int height_FTA, 
                   float* XI, float* YI, int* imgLabel);

void FS_CreateLabelledImage(int n_quadrangles, int* Quadrangles, 
                         int* landmarks, int width, int heigth, 
                         int* imgLabel);

py::array_t<int> FaceSwap_CPP(py::array_t<int> img_CAM, py::array_t<int> img_FTA,
              int width_CAM, int height_CAM, int width_FTA, int height_FTA,
              int n_quadrangles, py::array_t<int> Quadrangles, 
              py::array_t<int> landmarks_CAM, py::array_t<int> landmarks_FTA);

#endif //_FACESWAP_FUNC_H_