#include <pybind11/pybind11.h>
#include "faceswap_func_cuda.h"
#include "pybind_cuda.cuh"

using namespace pybind11;

PYBIND11_MODULE (faceswap_func_cuda, m) {
	m.doc() = "Librairie FaceSwap using cuda";

	m.def("CUDA_Sqr", &CUDA_Sqr);
	// Define function using :
	// module.def("__nom_fonction__", &__nom_fonction__)
	//m.def("FaceSwap", &FaceSwap);
}