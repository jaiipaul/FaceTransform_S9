#ifndef _ADDBEARD_FUNC_
#define _ADDBEARD_FUNC_

#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
namespace py = pybind11;

void generateParam(int* landmarks, int lenLand, int* moustacheLeftSide, int* moustacheTopSide, int* moustacheWidth);

py::array_t<uint8_t> AddBeard_CPP(py::array_t<uint8_t> img_CAM,
                              int width_CAM,int height_CAM,
                              py::array_t<int> landmarks_CAM);

#endif // _ADDBEARD_FUNC_