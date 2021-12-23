#ifndef _FACESWAP_FUNC_H_
#define _FACESWAP_FUNC_H_

#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

namespace py = pybind11;

void hello();
void addition(int a, int b);
void fillTab(py::list tab , int size);
void loadImage(py::array_t<int> img, int width, int height);
// Define function here 

void FindAllHomography(int n_quadrangles, int* Quadrangles, int* landmarks_src, int* landmarks_dst, double ** H);

void Find_rectangle(double *src,double *rect);
//TO FINISH
void ApplyAllHomography(int width, int height, int* imgLabel, double ** H, double* XI, double* YI);

void RecreateImage(int* imgOut, 
                   int* imgCAM ,int width_CAM, int height_CAM, 
                   int* imgFTA, int width_FTA, int height_FTA, 
                   double* XI, double* YI, int* imgLabel);

void CreateLabelledImage(int n_quadrangles, int* Quadrangles, 
                         int* landmarks, int width, int heigth, 
                         int* imgLabel);

py::array_t<int> FaceSwap(py::array_t<int> img_CAM, py::array_t<int> img_FTA,
              int width_CAM, int height_CAM, int width_FTA, int height_FTA,
              int n_quadrangles, py::array_t<int> Quadrangles, 
              py::array_t<int> landmarks_CAM, py::array_t<int> landmarks_FTA);

#endif //_FACESWAP_FUNC_H_