#ifndef _PYBIND_CUDA_H_
#define _PYBIND_CUDA_H_

py::array_t<int> CUDA_Sqr(py::array_t<int> img, int width, int height);

#endif // _PYBIND_CUDA_H_