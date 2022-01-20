#ifndef _BIGGERFACE_H_
#define _BIGGERFACE_H_

#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

namespace py = pybind11;

void BF_deformation(float* input, float* output);

void BF_FindAllHomography(int n_quadrangles, int* Quadrangles, int* landmarks_src, int* landmarks_dst, float** H);

void BF_Find_rectangle(float* src,float* rect);
//TO FINISH
void BF_ApplyAllHomography(int width, int height, int* imgLabel, float** H, float* XI, float* YI);

void BF_RecreateImage(int* imgOut, 
                   int* imgCAM ,int width_CAM, int height_CAM,  
                   float* XI, float* YI, int* imgLabel);

void BF_CreateLabelledImage(int n_quadrangles, int* Quadrangles, 
                         int* landmarks, int width, int heigth, 
                         int* imgLabel);

py::array_t<int> BiggerFace_CPP(py::array_t<int> img_CAM,
                                int width_CAM, int height_CAM,
                                int n_quadrangles, py::array_t<int> Quadrangles, 
                                py::array_t<int> landmarks_CAM);

#endif // _BIGGERFACE_H_