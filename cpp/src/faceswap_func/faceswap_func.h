#ifndef _FACESWAP_FUNC_H_
#define _FACESWAP_FUNC_H_

#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>



void hello();
void addition(int a, int b);
void fillTab(pybind11::list tab , int size);
void loadImage(pybind11::array_t<int> img, int width, int height);
// Define function here 

#endif //_FACESWAP_FUNC_H_