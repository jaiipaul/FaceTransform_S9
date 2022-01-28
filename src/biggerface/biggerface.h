#ifndef _BIGGERFACE_H_
#define _BIGGERFACE_H_

#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

namespace py = pybind11;

void BF_deformation(float* input, float* output);

py::array_t<int> BiggerFace_CPP(py::array_t<int> img_CAM,
                                int width_CAM, int height_CAM,
                                int n_quadrangles, py::array_t<int> Quadrangles, 
                                py::array_t<int> landmarks_CAM);

#endif // _BIGGERFACE_H_