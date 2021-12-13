cmake_minimum_required(VERSION 3.16)
project("Faceswap_func_cuda" LANGUAGES CXX CUDA)
set(CMAKE_CXX_STANDARD 17)

set(MODULE_SOURCE "${PROJECT_SOURCE_DIR}/src/faceswap_func_cuda")
set(INCLUDES "${PROJECT_SOURCE_DIR}/includes")

set(LIB_DIR "../../lib")
SET(LIBRARY_OUTPUT_PATH ${LIB_DIR}/cuda)
# pybind11
add_subdirectory(${INCLUDES}/pybind11)

# module
pybind11_add_module (
    faceswap_func_cuda
    ${MODULE_SOURCE}/pybind_cuda.cu 
    ${MODULE_SOURCE}/pybind_cuda.cpp
    ${MODULE_SOURCE}/wrapper.cpp
)

target_include_directories (
    faceswap_func_cuda
    PRIVATE ${MODULE_SOURCE}
)

set_target_properties( faceswap_func_cuda
                       PROPERTIES CUDA_SEPARABLE_COMPILATION OFF
                                  CUDA_ARCHITECTURES "50;")
