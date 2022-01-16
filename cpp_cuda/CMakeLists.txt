cmake_minimum_required(VERSION 3.16)
project("FaceTransform" LANGUAGES CXX CUDA)
set(CMAKE_CXX_STANDARD 17)
find_package(CUDA REQUIRED)

IF (CUDA_VERSION VERSION_LESS "11.0")
    set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} -gencode arch=compute_30,code=sm_30")
ENDIF()

set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} -gencode arch=compute_35,code=sm_35")

set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} -gencode arch=compute_50,code=sm_50")
set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} -gencode arch=compute_52,code=sm_52")

IF (CUDA_VERSION VERSION_GREATER "7.6")
    set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} -gencode arch=compute_60,code=sm_60")
    set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} -gencode arch=compute_61,code=sm_61")
    set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} -gencode arch=compute_62,code=sm_62")
ENDIF()

IF ((CUDA_VERSION VERSION_GREATER "9.0") OR (CUDA_VERSION VERSION_EQUAL "9.0"))
    set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} -gencode arch=compute_70,code=sm_70")
ENDIF()

IF ((CUDA_VERSION VERSION_GREATER "10.0") OR (CUDA_VERSION VERSION_EQUAL "10.0"))
    set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} -gencode arch=compute_75,code=sm_75")
ENDIF()

IF ((CUDA_VERSION VERSION_GREATER "11.0") OR (CUDA_VERSION VERSION_EQUAL "11.0"))
    set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} -gencode arch=compute_80,code=sm_80")
ENDIF()


# THIS LINE SHOULD BE CHANGE DEPENDING ON THE ARCHITECTURE OF THE GPU YOU INTEND TO USE
#set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} -gencode arch=compute_50,code=sm_50")

set(MODULE_SOURCE "${PROJECT_SOURCE_DIR}/src/")
set(INCLUDES "${PROJECT_SOURCE_DIR}/includes")

set(LIB_DIR "../../lib")
SET(LIBRARY_OUTPUT_PATH ${LIB_DIR}/)
# pybind11
add_subdirectory(${INCLUDES}/pybind11)

# cuda library
add_library (
    cuda_lib 
    ${MODULE_SOURCE}/faceswap/cuda_func.cu
)

target_include_directories (
    cuda_lib
    PRIVATE ${MODULE_SOURCE}/homography
    PRIVATE ${MODULE_SOURCE}/faceswap
    #PRIVATE ${MODULE_SOURCE}/biggerface 
)

#pybind module
pybind11_add_module( 
    FaceTransform MODULE
    ${MODULE_SOURCE}/homography/homographie.cpp
    ${MODULE_SOURCE}/faceswap/faceswap_func.cpp
    ${MODULE_SOURCE}/faceswap/faceswap_func_cuda.cpp
    ${MODULE_SOURCE}/wrapper.cpp
)
target_link_directories(
    FaceTransform
    PUBLIC ${LIB_DIR}
    PUBLIC ${PROJECT_SOURCE_DIR}/includes/opencv2/lib
)
target_link_libraries(FaceTransform 
    PUBLIC cuda_lib
    PUBLIC opencv_world455.lib)

target_include_directories (
    FaceTransform
    PRIVATE ${MODULE_SOURCE}/faceswap
    PRIVATE ${MODULE_SOURCE}/biggerface
    PRIVATE ${MODULE_SOURCE}/homography
    PRIVATE ${PROJECT_SOURCE_DIR}/includes/opencv2/include
)
