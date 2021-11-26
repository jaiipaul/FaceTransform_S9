#include <pybind11/pybind11.h>
#include "faceswap_func.h"

using namespace pybind11;

PYBIND11_MODULE (faceswap_func, m) {
	m.doc() = "Librairie FaceSwap";

	m.def("hello", &hello);
	m.def("addition", &addition);
	m.def("fillTab", &fillTab);
	m.def("loadImage", &loadImage);
	// Define function using :
	// module.def("__nom_fonction__", &__nom_fonction__)
	m.def("FaceSwap", &FaceSwap);
}