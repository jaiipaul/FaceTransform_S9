#ifndef _FACESWAP_FUNC_CUDA_H_
#define _FACESWAP_FUNC_CUDA_H_

#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

namespace py = pybind11;

py::array_t<int> CUDA_Sqr(py::array_t<int> img, int width, int height);

void Flatten(float** Tab2D, int w, int h, float* Tab1D);

py::array_t<int> FaceSwap_CUDA(py::array_t<int> img_CAM, py::array_t<int> img_FTA,
                               int width_CAM, int height_CAM, int width_FTA, int height_FTA,
                               int n_quadrangles, py::array_t<int> Quadrangles, 
                               py::array_t<int> landmarks_CAM, py::array_t<int> landmarks_FTA);
                   
#endif