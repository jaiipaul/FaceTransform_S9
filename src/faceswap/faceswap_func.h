#ifndef _FACESWAP_FUNC_H_
#define _FACESWAP_FUNC_H_

#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

namespace py = pybind11;

py::array_t<int> FaceSwap_CPP(py::array_t<int> img_CAM, py::array_t<int> img_FTA,
                              int width_CAM, int height_CAM, int width_FTA, int height_FTA,
                              int n_quadrangles, py::array_t<int> Quadrangles, 
                              py::array_t<int> landmarks_CAM, py::array_t<int> landmarks_FTA);
            
py::array_t<int> FaceSwap_CUDA(py::array_t<int> img_CAM, py::array_t<int> img_FTA,
                               int width_CAM, int height_CAM, int width_FTA, int height_FTA,
                               int n_quadrangles, py::array_t<int> Quadrangles, 
                               py::array_t<int> landmarks_CAM, py::array_t<int> landmarks_FTA);

#endif //_FACESWAP_FUNC_H_