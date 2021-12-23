#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>

#include "faceswap_func_cuda.h"
#include "pybind_cuda.cuh"

py::array_t<int> CUDA_Sqr(py::array_t<int> img, int width, int height) {
    
    py::buffer_info imgBuff = img.request();
    int* imgPtr = (int*)imgBuff.ptr;

    auto imgOut = py::array_t<int>(width*height);
    py::buffer_info imgOut_Buff = imgOut.request();
    int* imgOut_Ptr = (int*)(imgOut_Buff.ptr);

    gpu_square_main(imgPtr, width*height, imgOut_Ptr);
    std::cout << "CUDA done" << std::endl;
    return(imgOut);
}