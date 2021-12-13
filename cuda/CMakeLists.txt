cmake_minimum_required(VERSION 3.16)
project("Faceswap_func_cuda" LANGUAGES CXX CUDA)
find_package(CUDA REQUIRED)
find_package(PythonInterp 3.6 REQUIRED)
find_package(PythonLibs 3.6 REQUIRED)
find_package(Python3 COMPONENTS Interpreter Development)

set(CMAKE_CXX_STANDARD 17)

# need to be at least 30 or __shfl_down in reduce wont compile
IF (CUDA_VERSION VERSION_LESS "11.0")
    set(CUDA_NVCC_FLAGS "${CUDA_NVCC_FLAGS} -gencode arch=compute_30,code=sm_30")
ENDIF()

set(CUDA_NVCC_FLAGS "${CUDA_NVCC_FLAGS} -gencode arch=compute_35,code=sm_35")

set(CUDA_NVCC_FLAGS "${CUDA_NVCC_FLAGS} -gencode arch=compute_50,code=sm_50")
set(CUDA_NVCC_FLAGS "${CUDA_NVCC_FLAGS} -gencode arch=compute_52,code=sm_52")

IF (CUDA_VERSION VERSION_GREATER "7.6")
    set(CUDA_NVCC_FLAGS "${CUDA_NVCC_FLAGS} -gencode arch=compute_60,code=sm_60")
    set(CUDA_NVCC_FLAGS "${CUDA_NVCC_FLAGS} -gencode arch=compute_61,code=sm_61")
    set(CUDA_NVCC_FLAGS "${CUDA_NVCC_FLAGS} -gencode arch=compute_62,code=sm_62")
ENDIF()

IF ((CUDA_VERSION VERSION_GREATER "9.0") OR (CUDA_VERSION VERSION_EQUAL "9.0"))
    set(CUDA_NVCC_FLAGS "${CUDA_NVCC_FLAGS} -gencode arch=compute_70,code=sm_70")
ENDIF()

IF ((CUDA_VERSION VERSION_GREATER "10.0") OR (CUDA_VERSION VERSION_EQUAL "10.0"))
    set(CUDA_NVCC_FLAGS "${CUDA_NVCC_FLAGS} -gencode arch=compute_75,code=sm_75")
ENDIF()

IF ((CUDA_VERSION VERSION_GREATER "11.0") OR (CUDA_VERSION VERSION_EQUAL "11.0"))
    set(CUDA_NVCC_FLAGS "${CUDA_NVCC_FLAGS} -gencode arch=compute_80,code=sm_80")
ENDIF()


set(MODULE_SOURCE "${PROJECT_SOURCE_DIR}/src/faceswap_func_cuda")
set(INCLUDES "${PROJECT_SOURCE_DIR}/includes")

set(LIB_DIR "../../lib")
#SET(LIBRARY_OUTPUT_PATH ${LIB_DIR}/cuda)
# pybind11
add_subdirectory(${INCLUDES}/pybind11)

# cuda library
cuda_add_library (
    cuda_lib SHARED
    ${MODULE_SOURCE}/pybind_cuda.cu
)
target_link_libraries(cuda_lib
  cudart)
target_include_directories (
    cuda_lib
    PRIVATE ${MODULE_SOURCE}
)

pybind11_add_module (
    faceswap_func_cuda
    ${MODULE_SOURCE}/pybind_cuda.cpp
    ${MODULE_SOURCE}/wrapper.cpp
)
target_link_libraries(faceswap_func_cuda PRIVATE cuda_lib)
target_include_directories (
    faceswap_func_cuda
    PRIVATE ${MODULE_SOURCE}
)

set_target_properties( faceswap_func_cuda
                       PROPERTIES CUDA_SEPARABLE_COMPILATION ON
                                  CUDA_ARCHITECTURES "70;")
