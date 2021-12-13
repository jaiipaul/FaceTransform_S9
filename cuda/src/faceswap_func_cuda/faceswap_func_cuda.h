#ifndef _FACESWAP_FUNC_CUDA_H_
#define _FACESWAP_FUNC_CUDA_H_

#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

namespace py = pybind11;

py::array_t<int> CUDA_Sqr(py::array_t<int> img, int width, int height);

#endif