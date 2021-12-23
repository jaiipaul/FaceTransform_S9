cmake_minimum_required(VERSION 3.16)
project("Faceswap_func_cuda" LANGUAGES CXX CUDA)
find_package(CUDA REQUIRED)

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

set(CMAKE_CXX_STANDARD 17)

set(MODULE_SOURCE "${PROJECT_SOURCE_DIR}/src/faceswap_func_cuda")
set(INCLUDES "${PROJECT_SOURCE_DIR}/includes")

set(LIB_DIR "../../lib")
SET(LIBRARY_OUTPUT_PATH ${LIB_DIR}/cuda)
# pybind11
add_subdirectory(${INCLUDES}/pybind11)

# cuda library
add_library (
    cuda_lib 
    ${MODULE_SOURCE}/pybind_cuda.cu
)

target_include_directories (
    cuda_lib
    PRIVATE ${MODULE_SOURCE}
)

#set_target_properties(cuda_lib PROPERTIES
    #CUDA_SEPARABLE_COMPILATION ON
    #CUDA_ARCHITECTURES "80; 70;"
#    )

#pybind module
pybind11_add_module( 
    faceswap_func_cuda MODULE
    ${MODULE_SOURCE}/faceswap_func_cuda.cpp
    ${MODULE_SOURCE}/wrapper.cpp
)
target_link_libraries(faceswap_func_cuda PUBLIC cuda_lib)

target_include_directories (
    faceswap_func_cuda
    PRIVATE ${MODULE_SOURCE}
)



