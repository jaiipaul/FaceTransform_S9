#include <pybind11/pybind11.h>
#include "faceswap_func.h"
#include "faceswap_func_cuda.h"
#include "pybind_cuda.cuh"

using namespace pybind11;

PYBIND11_MODULE (faceswap_func, m) {
	m.doc() = "Librairie FaceSwap using cpp and cuda";

	m.def("CUDA_Sqr", &CUDA_Sqr);
	// Define function using :
	// module.def("__nom_fonction__", &__nom_fonction__)
	m.def("FaceSwap_CPP",  &FaceSwap_CPP);
	m.def("FaceSwap_CUDA", &FaceSwap_CUDA);
}