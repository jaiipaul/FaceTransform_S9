#include <pybind11/pybind11.h>
#include "faceswap_func.h"

PYBIND11_MODULE (faceswap_func, module) {
	module.doc() = "Librairie FaceSwap";

	module.def("hello", &hello);
	module.def("addition", &addition);
	// Define function using :
	// module.def("__nom_fonction__", &__nom_fonction__)
}