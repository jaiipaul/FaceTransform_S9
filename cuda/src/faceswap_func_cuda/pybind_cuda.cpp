#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include "pybind_cuda.h"
#include "pybind_cuda.cuh"

#include <pybind11/pybind11.h>

namespace py = pybind11;

py::array_t<int> CUDA_Sqr(py::array_t<int> img, int width, int height) {
    
    py::buffer_info imgBuff = img.request();
    int* imgPtr = (int*)imgBuff.ptr;

    auto imgOut = py::array_t<int>(width_CAM*height_CAM*3);
    py::buffer_info imgOut_Buff = imgOut.request();
    int* imgOut_Ptr = (int*)(imgOut_Buff.ptr);

    gpu_square_main(imgPtr, width*height, imgOut);

    return(imgOut);
}