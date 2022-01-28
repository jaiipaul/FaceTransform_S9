#include <pybind11/pybind11.h>

#include "faceswap_func.h"
#include "biggerface.h"
#include "addBeard_func.h"

using namespace pybind11;

PYBIND11_MODULE (FaceTransform, m) {
	m.doc() = "Librairie Face Transform using cpp and cuda";

	// Define function using :
	// module.def("__nom_fonction__", &__nom_fonction__)
	m.def("FaceSwap_CPP",  &FaceSwap_CPP);
	m.def("FaceSwap_CUDA", &FaceSwap_CUDA);
	m.def("BiggerFace_CPP", &BiggerFace_CPP);
	m.def("AddBeard_CPP",  &AddBeard_CPP);
}